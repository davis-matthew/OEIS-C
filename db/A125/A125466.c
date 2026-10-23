/*
 * A125466  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Number of base 31 circular n‑digit numbers with adjacent
 *            digits differing by <=8. a(n) equals trace(A^n) where A is the
 *            31×31 0/1 adjacency matrix |i−j|≤8. The sequence satisfies a
 *            linear recurrence of order ≤31. The recurrence is computed once
 *            using Berlekamp–Massey over a large prime and...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

static const long long MOD = 1000000007LL;

static bool ready = false;
static size_t coeff_len = 0;
static int64_t *coeff = NULL;
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* modular inverse modulo MOD (prime) */
static long long mod_inv(long long a) {
    long long b = MOD, u = 1, v = 0;
    while (b != 0) {
        long long t = a / b;
        a -= t * b;  long long tmp = a; a = b; b = tmp;
        u -= t * v;  tmp = u; u = v; v = tmp;
    }
    if (a != 1) return 1;                /* should not happen */
    if (u < 0) u += MOD;
    return u;
}

/* Berlekamp–Massey over modulo MOD.  Input s[0..N-1].
   Returns the minimal order L and stores the recurrence
   coefficients (excluding the leading 1) in ret[0..L-1]. */
static int bm_seq(const long long *s, int N, long long *ret) {
    long long C[64] = {0}, B[64] = {0};
    int L = 0, m = 1;
    long long b = 1;
    C[0] = B[0] = 1;

    for (int n = 0; n < N; ++n) {
        long long d = s[n];
        for (int i = 1; i <= L; ++i) {
            d = (d + C[i] * s[n - i]) % MOD;
            if (d < 0) d += MOD;
        }
        if (d == 0) {
            ++m;
            continue;
        }
        long long coef = d * mod_inv(b) % MOD;
        long long oldC[64];
        for (int i = 0; i < 64; ++i) oldC[i] = C[i];
        for (int i = 0; i <= L; ++i) {
            long long val = (C[i + m] - coef * B[i]) % MOD;
            if (val < 0) val += MOD;
            C[i + m] = val;
        }
        if (2 * L <= n) {
            L = n + 1 - L;
            for (int i = 0; i < 64; ++i) B[i] = oldC[i];
            b = d;
            m = 1;
        } else
            ++m;
    }
    for (int i = 0; i < L; ++i) ret[i] = C[i + 1];
    return L;
}

/* Pre‑compute the first 62 terms and determine the recurrence. */
static void compute_initial(void) {
    /* Build adjacency lists */
    int deg[31];
    int neigh[31][17];          /* maximum degree is 17 */
    for (int i = 0; i < 31; ++i) {
        int cnt = 0;
        for (int j = 0; j < 31; ++j)
            if (abs(i - j) <= 8)
                neigh[i][cnt++] = j;
        deg[i] = cnt;
    }

    /* Compute the first 62 trace values (a[0]..a[61]) */
    seq_t init[62];
    init[0] = 1;

    seq_t B[31][31];
    seq_t Bnew[31][31];
    for (int i = 0; i < 31; ++i)
        for (int j = 0; j < 31; ++j)
            B[i][j] = (i == j) ? (seq_t)1 : 0;

    for (int step = 1; step <= 61; ++step) {
        /* zero Bnew */
        for (int i = 0; i < 31; ++i)
            for (int j = 0; j < 31; ++j)
                Bnew[i][j] = 0;

        /* Bnew = B * A */
        for (int i = 0; i < 31; ++i) {
            for (int k = 0; k < 31; ++k) {
                seq_t val = B[i][k];
                if (val == 0) continue;
                for (int idx = 0; idx < deg[k]; ++idx) {
                    int j = neigh[k][idx];
                    Bnew[i][j] = Bnew[i][j] + val;
                }
            }
        }

        /* copy Bnew to B */
        for (int i = 0; i < 31; ++i)
            for (int j = 0; j < 31; ++j)
                B[i][j] = Bnew[i][j];

        /* trace */
        seq_t tr = 0;
        for (int i = 0; i < 31; ++i)
            tr = tr + B[i][i];
        init[step] = tr;
    }

    /* Prepare sequence modulo MOD for Berlekamp–Massey */
    long long modseq[62];
    for (int i = 0; i < 62; ++i) {
        long long v = (long long)(init[i] % MOD);
        if (v < 0) v += MOD;
        modseq[i] = v;
    }

    long long bm[64];
    coeff_len = bm_seq(modseq, 62, bm);

    /* Convert recurrence to signed integer coefficients */
    coeff = (int64_t *)malloc(coeff_len * sizeof(int64_t));
    for (int i = 0; i < coeff_len; ++i) {
        long long cmod = (-bm[i]) % MOD;
        if (cmod < 0) cmod += MOD;
        if (cmod > MOD / 2) cmod -= MOD;   /* negative value */
        coeff[i] = (int64_t)cmod;
    }

    /* Initialize cache with the first 'coeff_len' terms */
    cache_cap = coeff_len > 0 ? coeff_len : 1;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (int i = 0; i < coeff_len; ++i)
        cache[i] = init[i];
    cache_len = coeff_len;
}

/* Public function as required */
seq_t a(idx_t n) {
    if (!ready) {
        compute_initial();
        ready = true;
    }

    if (n < cache_len)
        return cache[n];

    /* Expand cache up to n */
    while (cache_len <= n) {
        /* Ensure enough capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next term using the recurrence */
        idx_t idx = cache_len;
        seq_t pos = 0;
        seq_t neg = 0;
        for (size_t k = 0; k < coeff_len; ++k) {
            seq_t term = mul_small(cache[idx - 1 - k],
                                   (unsigned int)(coeff[k] >= 0 ? coeff[k] : -coeff[k]));
            if (coeff[k] >= 0)
                pos = pos + term;
            else
                neg = neg + term;
        }
        cache[idx] = pos - neg;
        ++cache_len;
    }

    return cache[n];
}
