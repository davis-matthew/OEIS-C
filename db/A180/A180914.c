/*
 * A180914  Number of sequences of n integers p(i) i=0..n-1 with 0<=p(i)<=9*i
 *          and -9<p(i)-p(i-1)<=9.
 * https://oeis.org/A180914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer sequences p(i), i = 0..n-1 satisfying 0
 *            ≤ p(i) ≤ 9*i and -9 < p(i) – p(i-1) ≤ 9 (OEIS A180914). The
 *            sequence satisfies a linear recurrence of small order; the
 *            program computes the first few hundred terms, extracts the
 *            recurrence with Berlekamp–Massey over two primes, reco...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- auxiliary modular arithmetic ---------- */

static uint32_t mod_pow32(uint32_t a, uint32_t e, uint32_t mod)
/* fast exponentiation: a^e mod mod (mod is prime) */
{
    uint64_t r = 1;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1) r = (r * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return (uint32_t)r;
}

static uint32_t mod_inv32(uint32_t a, uint32_t mod)
/* multiplicative inverse of a modulo prime mod */
{
    return mod_pow32(a, mod - 2, mod);
}

/* ---------- Berlekamp–Massey over a prime ---------- */

static void bm_mod(const uint32_t *s, size_t n, uint32_t mod,
                   uint32_t *rec, size_t *rec_len)
{
    size_t L = 0, m = 1;
    uint32_t *C = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    C[0] = 1;
    B[0] = 1;
    uint32_t b = 1;  /* previous discrepancy */

    for (size_t i = 0; i < n; ++i) {
        uint32_t d = s[i];
        for (size_t j = 1; j <= L; ++j) {
            d = (d + (uint64_t)C[j] * s[i - j]) % mod;
        }

        if (d == 0) {
            ++m;
            continue;
        }

        uint32_t coef = (uint64_t)d * mod_inv32(b, mod) % mod;
        uint32_t *T = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
        memcpy(T, C, (n + 1) * sizeof(uint32_t));

        for (size_t j = 0; j <= L; ++j) {
            if (B[j] == 0) continue;
            uint32_t sub = (uint64_t)coef * B[j] % mod;
            size_t idx = j + m;
            if (idx <= n) {
                C[idx] = (C[idx] + mod - sub) % mod;
            }
        }

        if (2 * L <= i) {
            L = i + 1 - L;
            memcpy(B, T, (n + 1) * sizeof(uint32_t));
            b = d;
            m = 1;
        } else {
            ++m;
        }
        free(T);
    }

    *rec_len = L;
    for (size_t i = 0; i < L; ++i) {
        /* recurrence: a(n) = -C[1]*a(n-1)-...-C[L]*a(n-L) (mod) */
        if (C[i + 1] == 0)
            rec[i] = 0;
        else
            rec[i] = mod - C[i + 1];
    }

    free(C);
    free(B);
}

/* ---------- reconstruction of integer coefficient from two residues ---------- */

static int64_t rebuild_int64_from_resid(const uint32_t *resid,
                                        const uint32_t *primes, size_t m)
{
    /* Garner for two primes, product < 2^63 */
    seq_t x = (seq_t)resid[0];
    seq_t M = (seq_t)primes[0];

    for (size_t k = 1; k < m; ++k) {
        uint32_t M_mod = (uint32_t)(M % primes[k]);
        uint32_t inv = mod_inv32(M_mod, primes[k]);

        uint32_t x_mod = (uint32_t)(x % primes[k]);
        uint32_t t = (resid[k] + primes[k] - x_mod) % primes[k];
        t = (uint32_t)((uint64_t)t * inv % primes[k]);

        x = x + (seq_t)t * M;
        M = M * (seq_t)primes[k];
    }

    seq_t prod = (seq_t)primes[0] * (seq_t)primes[1];
    seq_t half = prod / 2;
    if (x > half) {
        seq_t diff = x - prod;          /* diff is positive, < prod/2 */
        return -(int64_t)diff;          /* convert to signed negative */
    } else {
        return (int64_t)x;
    }
}

/* ---------- DP to compute initial terms ---------- */

static void compute_initial_terms(idx_t M, seq_t *init)
{
    /* dp_prev holds counts for level i-1 */
    seq_t *dp_prev = (seq_t *)calloc(1, sizeof(seq_t));
    dp_prev[0] = (seq_t)1;
    size_t prev_size = 1;

    init[0] = (seq_t)1;                /* a(1) = 1 */

    for (idx_t step = 1; step < M; ++step) {
        size_t new_size = 9 * step + 1;
        seq_t *dp_next = (seq_t *)calloc(new_size, sizeof(seq_t));

        /* prefix sums of dp_prev */
        seq_t *pref = (seq_t *)malloc(prev_size * sizeof(seq_t));
        if (prev_size > 0) {
            pref[0] = dp_prev[0];
            for (size_t i = 1; i < prev_size; ++i)
                pref[i] = pref[i - 1] + dp_prev[i];
        }

        seq_t sum = 0;
        for (size_t v = 0; v < new_size; ++v) {
            size_t low = (v >= 9) ? v - 9 : 0;
            size_t high = (v + 8 < prev_size) ? v + 8 : prev_size - 1;

            seq_t val = pref[high];
            if (low > 0) val -= pref[low - 1];
            dp_next[v] = val;
            sum += val;
        }
        init[step] = sum;

        free(pref);
        free(dp_prev);
        dp_prev = dp_next;
        prev_size = new_size;
    }
    free(dp_prev);
}

/* ---------- global state ---------- */

struct {
    seq_t *init;         /* first computed terms a(1)..a(M) */
    size_t init_len;     /* M                     */
    int64_t *rec;        /* recurrence coefficients (signed) */
    size_t rec_len;      /* order of recurrence         */
} state = {0};

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for offset 1 */

    if (state.init == NULL) {       /* first call: compute terms and recurrence */
        const size_t M = 200;                /* number of initial terms to compute */
        const uint32_t primes[2] = {1000000007U, 1000000009U};

        /* compute initial terms */
        state.init = (seq_t *)malloc(M * sizeof(seq_t));
        compute_initial_terms(M, state.init);
        state.init_len = M;

        /* prepare residues modulo the two primes */
        uint32_t *s1 = (uint32_t *)malloc(M * sizeof(uint32_t));
        uint32_t *s2 = (uint32_t *)malloc(M * sizeof(uint32_t));
        for (size_t i = 0; i < M; ++i) {
            s1[i] = (uint32_t)(state.init[i] % primes[0]);
            s2[i] = (uint32_t)(state.init[i] % primes[1]);
        }

        /* Berlekamp–Massey for each prime */
        size_t len1, len2;
        uint32_t *rec1 = (uint32_t *)malloc(M * sizeof(uint32_t));
        uint32_t *rec2 = (uint32_t *)malloc(M * sizeof(uint32_t));
        bm_mod(s1, M, primes[0], rec1, &len1);
        bm_mod(s2, M, primes[1], rec2, &len2);
        free(s1);
        free(s2);

        /* assume same order; if not, take the larger */
        size_t L = (len1 > len2) ? len1 : len2;
        state.rec_len = L;
        state.rec = (int64_t *)malloc(L * sizeof(int64_t));

        for (size_t j = 0; j < L; ++j) {
            uint32_t r0 = (j < len1) ? rec1[j] : 0;
            uint32_t r1 = (j < len2) ? rec2[j] : 0;
            uint32_t residues[2] = {r0, r1};
            state.rec[j] = rebuild_int64_from_resid(residues, primes, 2);
        }

        free(rec1);
        free(rec2);
    }

    /* direct lookup for early terms */
    if ((size_t)n <= state.init_len)
        return state.init[n - 1];

    /* evaluate recurrence from state.init_len up to n-1 */
    size_t cur = state.init_len;      /* next index to compute  (0‑based) */
    size_t L = state.rec_len;
    seq_t *buf = (seq_t *)malloc(L * sizeof(seq_t));

    /* fill buffer with the last L terms of the known prefix */
    for (size_t j = 0; j < L; ++j)
        buf[j] = state.init[cur - L + j];      /* ensures cur >= L */

    /* compute subsequent terms */
    for (; cur <= n - 1; ++cur) {
        seq_t term = 0;
        for (size_t j = 0; j < L; ++j) {
            int64_t coeff = state.rec[j];
            size_t idx = L - 1 - j;          /* buf[index] = a[cur-1-j] */
            if (coeff >= 0)
                term += buf[idx] * (seq_t)coeff;
            else
                term -= buf[idx] * (seq_t)(-coeff);
        }
        /* shift buffer */
        for (size_t k = 0; k < L - 1; ++k)
            buf[k] = buf[k + 1];
        buf[L - 1] = term;
    }

    seq_t res = buf[L - 1];
    free(buf);
    return res;
}
