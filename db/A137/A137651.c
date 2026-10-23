/*
 * A137651  Triangle read by rows: T(n,k) is the number of primitive
 *          (aperiodic) word structures of length n using exactly k different
 *          symbols.
 * https://oeis.org/A137651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{d|r} μ(r/d) * Stirling2(d, k) where r, k are obtained
 *            from the triangular representation of n: find minimal r with
 *            r(r+1)/2 ≥ n, set k = n – r(r–1)/2
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 140, u64 n <= 358,
 *            u128 n <= 957, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a small BigInt type to handle arbitrary‐precision answers
   for seq_t.  The algorithm stores the Stirling numbers S(i,j) for 0≤i≤r, 0≤j≤k
   in a 1‑D array and accumulates the Mobius weighted sum with separate positive
   and negative accumulators to avoid subtraction underflows. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_from_str.h"

/* ---------- Small Big Integer implementation (unsigned decimal) ---------- */
#define BASE 1000000000U              /* 1e9 decimal base for BigInt */
typedef struct {
    size_t len;          /* number of used limbs (BASE) */
    uint32_t *d;         /* little‑endian limbs, 0 ≤ limb < BASE */
} BigInt;

static void big_init(BigInt *x) { x->len = 0; x->d = NULL; }

static void big_set_zero(BigInt *x) {
    if (x->d) free(x->d);
    x->len = 0; x->d = NULL;
}

static void big_from_uint32(BigInt *x, uint32_t v) {
    if (v == 0) { big_set_zero(x); return; }
    x->len = 1; x->d = (uint32_t*)malloc(sizeof(uint32_t));
    x->d[0] = v;
}

static void big_free(BigInt *x) { if (x->d) free(x->d); x->len = 0; x->d = NULL; }

static void big_copy(BigInt *dst, const BigInt *src) {
    if (dst == src) return;
    if (dst->d) free(dst->d);
    dst->len = src->len;
    if (src->len == 0) { dst->d = NULL; return; }
    dst->d = (uint32_t*)malloc(src->len * sizeof(uint32_t));
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

static void big_add(const BigInt *a, const BigInt *b, BigInt *res) {
    size_t n = a->len > b->len ? a->len : b->len;
    uint64_t carry = 0, sum;
    uint32_t *dig = (uint32_t*)malloc((n + 1) * sizeof(uint32_t));
    for (size_t i = 0; i < n; ++i) {
        sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        dig[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) { dig[n] = (uint32_t)carry; n++; }
    big_free(res);
    res->len = n; res->d = dig;
}

static void big_sub(const BigInt *a, const BigInt *b, BigInt *res) {
    /* assume a >= b */
    size_t n = a->len;
    int64_t carry = 0, diff;
    uint32_t *dig = (uint32_t*)malloc(n * sizeof(uint32_t));
    for (size_t i = 0; i < n; ++i) {
        diff = (int64_t)a->d[i] - carry - (i < b->len ? b->d[i] : 0);
        if (diff < 0) { diff += BASE; carry = 1; }
        else          { carry = 0; }
        dig[i] = (uint32_t)diff;
    }
    while (n > 0 && dig[n-1] == 0) n--;
    big_free(res);
    res->len = n; res->d = dig;
}

static void big_mul_small(const BigInt *a, uint32_t m, BigInt *res) {
    if (m == 0 || a->len == 0) { big_set_zero(res); return; }
    size_t n = a->len;
    uint64_t carry = 0, prod;
    uint32_t *dig = (uint32_t*)malloc((n + 1) * sizeof(uint32_t));
    for (size_t i = 0; i < n; ++i) {
        prod = (uint64_t)a->d[i] * m + carry;
        dig[i] = (uint32_t)(prod % BASE);
        carry  = prod / BASE;
    }
    if (carry) { dig[n] = (uint32_t)carry; n++; }
    big_free(res);
    res->len = n; res->d = dig;
}

/* convert BigInt to decimal string (buffer must be large enough) */
static void big_to_dec_str(const BigInt *x, char *buf, size_t buflen) {
    if (x->len == 0) { if (buflen >= 2) { buf[0]='0'; buf[1]='\0'; } return; }
    size_t i = x->len;
    int n = snprintf(buf, buflen, "%u", x->d[i-1]);
    size_t pos = (size_t)n;
    for (int j = (int)i-2; j >= 0; --j) {
        if (pos + 10 >= buflen) break;
        snprintf(buf + pos, buflen - pos, "%09u", x->d[j]);
        pos = strlen(buf);
    }
}

/* ---------- μ(n) computation up to MAX_R ---------- */
#define MAX_R 200                 /* sufficient for n <= 10000 */
static int mu_arr[MAX_R + 1];
static bool mu_init_done = false;

static void init_mu(void) {
    if (mu_init_done) return;
    for (int i = 0; i <= MAX_R; ++i) mu_arr[i] = 1;
    bool is_prime[MAX_R + 1];
    for (int i = 0; i <= MAX_R; ++i) is_prime[i] = true;
    for (int p = 2; p <= MAX_R; ++p) {
        if (!is_prime[p]) continue;
        for (int m = p; m <= MAX_R; m += p) {
            mu_arr[m] = -mu_arr[m];
            if (m + p <= MAX_R) is_prime[m + p] = false;
        }
        long long sq = 1LL * p * p;
        if (sq <= MAX_R) {
            for (int m = (int)sq; m <= MAX_R; m += (int)sq) mu_arr[m] = 0;
        }
    }
    mu_arr[1] = 1;
    mu_init_done = true;
}

/* ---------- Stirling numbers S(n,k) (second kind) ----------- */
static void compute_stirling(BigInt *S, int n, int k) {
    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= k; ++j)
            big_init(&S[i*(k+1)+j]);

    big_from_uint32(&S[0*(k+1)+0], 1);

    BigInt tmp_mul, tmp_sum;
    big_init(&tmp_mul);
    big_init(&tmp_sum);

    for (int i = 1; i <= n; ++i) {
        big_set_zero(&S[i*(k+1)+0]);   /* S(i,0)=0 */
        for (int j = 1; j <= k; ++j) {
            if (j > i) { big_set_zero(&S[i*(k+1)+j]); continue; }
            big_mul_small(&S[(i-1)*(k+1)+j], (uint32_t)j, &tmp_mul);
            big_add(&S[(i-1)*(k+1)+j-1], &tmp_mul, &tmp_sum);
            big_copy(&S[i*(k+1)+j], &tmp_sum);
        }
    }
    big_free(&tmp_mul);
    big_free(&tmp_sum);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return seq_from_str("0");

    /* Find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;               /* 1 <= k <= r */

    init_mu();

    /* Compute Stirling numbers up to r,k */
    size_t size = (size_t)(r + 1) * (size_t)(k + 1);
    BigInt *S = (BigInt*)malloc(size * sizeof(BigInt));
    if (!S) return seq_from_str("0");
    compute_stirling(S, (int)r, (int)k);

    /* Accumulate μ‑weighted sum using separate positive and negative parts */
    BigInt pos, neg, tmp;
    big_init(&pos); big_init(&neg); big_init(&tmp);

    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d != 0) continue;
        /* divisor d */
        int mu1 = mu_arr[r / d];
        if (mu1 != 0) {
            BigInt const *S_val = &S[d * (k + 1) + k];
            if (mu1 == 1) { big_add(&pos, S_val, &tmp); big_copy(&pos, &tmp); }
            else          { big_add(&neg, S_val, &tmp); big_copy(&neg, &tmp); }
        }
        idx_t d2 = r / d;
        if (d2 != d) {
            int mu2 = mu_arr[r / d2];
            if (mu2 != 0) {
                BigInt const *S_val = &S[d2 * (k + 1) + k];
                if (mu2 == 1) { big_add(&pos, S_val, &tmp); big_copy(&pos, &tmp); }
                else          { big_add(&neg, S_val, &tmp); big_copy(&neg, &tmp); }
            }
        }
    }

    /* total = pos - neg  (should be non‑negative) */
    BigInt total;
    big_init(&total);
    if (big_cmp(&pos, &neg) >= 0) {
        big_sub(&pos, &neg, &tmp);
        big_copy(&total, &tmp);
    } else {
        /* In theory this should not happen; treat as zero */
        big_set_zero(&total);
    }

    /* Convert BigInt to decimal string and then to seq_t */
    char buf[2048];
    big_to_dec_str(&total, buf, sizeof(buf));
    seq_t result = seq_from_str(buf);

    /* Clean up */
    for (int i = 0; i <= r; ++i)
        for (int j = 0; j <= k; ++j)
            big_free(&S[i*(k+1)+j]);
    free(S);
    big_free(&pos); big_free(&neg); big_free(&tmp); big_free(&total);

    return result;
}
