/*
 * A073460  a(n) = smallest solution x to n + pi(x) = phi(x), or 0 if no such
 *          solution exists.
 * https://oeis.org/A073460
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 7606, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Min{x ; phi(x) - pi(x) = n}, returning 0 if no such x exists. */
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t *diff_to_x = NULL;   /* diff + 2 -> smallest x */
static const idx_t MAX_X = 200000; /* upper bound for x-value */
static const int  MAX_DIFF = 20000;/* maximum difference we care about */

static void init_sequence(void)
{
    if (initialized) return;

    /* ---------- Build smallest prime factor array ---------- */
    unsigned int *spf = (unsigned int *)malloc((MAX_X + 1) * sizeof(unsigned int));
    for (idx_t i = 0; i <= MAX_X; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= MAX_X; ++i) {
        if (spf[i] == 0) {
            spf[i] = (unsigned int)i;
            if (i * (idx_t)i <= MAX_X)  /* avoid overflow */
                for (idx_t j = i * i; j <= MAX_X; j += i)
                    if (spf[j] == 0) spf[j] = (unsigned int)i;
        }
    }

    /* ---------- Compute phi for all numbers up to MAX_X ---------- */
    unsigned int *phi = (unsigned int *)malloc((MAX_X + 1) * sizeof(unsigned int));
    phi[0] = 0;
    phi[1] = 1;
    for (idx_t n = 2; n <= MAX_X; ++n) {
        unsigned int p = spf[n];
        unsigned int m = (unsigned int)(n / p);
        if (m % p == 0)
            phi[n] = phi[m] * p;
        else
            phi[n] = phi[m] * (p - 1);
    }

    /* ---------- Compute prime counting function pi ---------- */
    unsigned int *pi = (unsigned int *)malloc((MAX_X + 1) * sizeof(unsigned int));
    pi[0] = 0;
    for (idx_t n = 1; n <= MAX_X; ++n) {
        pi[n] = pi[n - 1] + ((n >= 2 && spf[n] == (unsigned int)n) ? 1 : 0);
    }

    /* ---------- Prepare mapping from diff to smallest x ---------- */
    diff_to_x = (seq_t *)malloc((MAX_DIFF + 3) * sizeof(seq_t)); /* indices [-2 .. MAX_DIFF] */
    for (int i = 0; i < MAX_DIFF + 3; ++i) diff_to_x[i] = 0;

    for (idx_t x = 1; x <= MAX_X; ++x) {
        int diff = (int)((int64_t)phi[x] - (int64_t)pi[x]); /* phi - pi */
        if (diff < -2 || diff > MAX_DIFF) continue;
        size_t idx = (size_t)(diff + 2);
        if (diff_to_x[idx] == 0) diff_to_x[idx] = (seq_t)x;
    }

    /* ---------- Clean up ---------- */
    free(spf);
    free(phi);
    free(pi);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    /* n can be less than -2; behavior unspecified for such indices. */
    if (n < -2) return 0;

    size_t idx = (size_t)(n + 2);
    if (idx >= (size_t)(MAX_DIFF + 3)) return 0; /* out of precomputed range */

    return diff_to_x[idx];
}
