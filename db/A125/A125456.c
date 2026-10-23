/*
 * A125456  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n≥1, a(0)=1 (A: 21×21 adjacency matrix *
 *            with A[i][j] = 1 if |i−j|≤8, else 0 )
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   mod_norm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_norm.h"

#define K 21
#define PRIME_COUNT 3

static int64_t modinv64(int64_t a, int64_t mod) {
    int64_t b = mod, u = 1, v = 0;
    while (b) {
        int64_t q = a / b;
        int64_t t = a - q * b; a = b; b = t;
        t = u - q * v; u = v; v = t;
    }
    if (u < 0) u += mod;
    return u;
}

/* Berlekamp–Massey over a prime field */
static void bm_mod(const int64_t *s, int n, int64_t mod, int64_t *rec_out) {
    int64_t C[K+1] = {0}, B[K+1] = {0};
    C[0] = 1; B[0] = 1;
    int L = 0, m = 1, b = 1;
    for (int i = 0; i < n; ++i) {
        int64_t d = s[i];
        for (int j = 0; j < L; ++j)
            d = mod_norm(d + C[j+1]*s[i-j-1], mod);
        if (d == 0) { m++; continue; }
        int64_t coef = d * modinv64(b, mod) % mod;
        if (2*L <= i) {
            for (int j = 0; j <= L; ++j) B[j] = C[j];
            b = d;
            L = i + 1 - L;
            m = 1;
        } else {
            m++;
        }
        for (int j = 0; j < L; ++j) {
            int64_t idx = j + m;
            if (idx > K) break;
            C[idx] = mod_norm(C[idx] - coef*B[j], mod);
        }
    }
    for (int i = 0; i < L; ++i)
        rec_out[i] = mod_norm(mod - C[i+1], mod);
    for (int i = L; i < K; ++i)
        rec_out[i] = 0;
}

/* Convolution + reduction modulo the characteristic polynomial */
static void combine(const seq_t *a, const seq_t *b, const seq_t *rec, seq_t *res) {
    seq_t tmp[2*K] = {0};
    for (int i = 0; i < K; ++i) if (a[i]) {
        for (int j = 0; j < K; ++j) if (b[j]) {
            tmp[i+j] += a[i] * b[j];
        }
    }
    for (int i = 2*K-1; i >= K; --i) {
        if (tmp[i] == 0) continue;
        for (int j = 0; j < K; ++j) {
            tmp[i-1-j] += rec[j] * tmp[i];
        }
        tmp[i] = 0;
    }
    for (int i = 0; i < K; ++i) res[i] = tmp[i];
}

/* Compute initial terms (a[0..K-1]) with exact arithmetic */
static void init_sequence(seq_t *init) {
    seq_t B[K][K] = {0};
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            if ((i > j ? i-j : j-i) <= 8) B[i][j] = 1;

    init[0] = 1;
    seq_t cur[K][K];
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            cur[i][j] = B[i][j];

    for (int n = 1; n < K; ++n) {
        seq_t tr = 0;
        for (int i = 0; i < K; ++i) tr += cur[i][i];
        init[n] = tr;

        /* cur = cur * B */
        seq_t nxt[K][K] = {0};
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j) {
                seq_t sum = 0;
                for (int l = 0; l < K; ++l)
                    sum += cur[i][l] * B[l][j];
                nxt[i][j] = sum;
            }
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                cur[i][j] = nxt[i][j];
    }
}

/* Compute recurrence coefficients via CRT */
static void compute_recurrence(seq_t *rec, const seq_t *init) {
    static const int64_t primes[PRIME_COUNT] = {1000003LL, 1000033LL, 1000037LL};
    int64_t terms[2*K];
    int64_t rec_mod[PRIME_COUNT][K];

    for (int p = 0; p < PRIME_COUNT; ++p) {
        int64_t mod = primes[p];
        /* adjacency matrix modulo mod */
        int64_t B[K][K];
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                B[i][j] = ((i > j ? i-j : j-i) <= 8) ? 1 % mod : 0;

        int64_t cur[K][K];
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                cur[i][j] = B[i][j];

        terms[0] = 1 % mod;                /* a(0) */
        for (int n = 1; n < 2*K; ++n) {
            int64_t tr = 0;
            for (int i = 0; i < K; ++i)
                tr = (tr + cur[i][i]) % mod;
            terms[n] = tr;
            /* cur = cur * B */
            int64_t nxt[K][K] = {0};
            for (int i = 0; i < K; ++i)
                for (int j = 0; j < K; ++j) {
                    int64_t sum = 0;
                    for (int l = 0; l < K; ++l)
                        sum = (sum + cur[i][l]*B[l][j]) % mod;
                    nxt[i][j] = sum;
                }
            for (int i = 0; i < K; ++i)
                for (int j = 0; j < K; ++j)
                    cur[i][j] = nxt[i][j];
        }

        bm_mod(terms, 2*K, mod, rec_mod[p]);
    }

    /* Chinese Remainder Theorem to recover integer rec */
    for (int i = 0; i < K; ++i) {
        __int128 res = 0;
        __int128 m = 1;
        for (int p = 0; p < PRIME_COUNT; ++p) {
            int64_t mod = primes[p];
            int64_t r = rec_mod[p][i];
            int64_t inv = modinv64((int64_t)(m % mod), mod);
            int64_t ttmp = (r - (int64_t)(res % mod) + mod) % mod;
            ttmp = (ttmp * inv) % mod;
            res += m * ttmp;
            m *= mod;
        }
        rec[i] = (seq_t)res;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    static bool ready = false;
    static seq_t init[K];
    static seq_t rec[K];

    if (!ready) {
        init_sequence(init);
        compute_recurrence(rec, init);
        ready = true;
    }

    if (n < K) return init[n];

    idx_t exp = n;
    seq_t res[K] = {0}, base[K] = {0};
    res[0] = 1;          /* polynomial 1   (X^0) */
    if (K > 1) base[1] = 1;/* polynomial X   (X^1) */

    while (exp) {
        if (exp & 1) combine(res, base, rec, res);
        combine(base, base, rec, base);
        exp >>= 1;
    }

    seq_t ans = 0;
    for (int i = 0; i < K; ++i)
        ans += res[i] * init[i];
    return ans;
}
