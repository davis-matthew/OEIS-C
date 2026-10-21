/*
 * A069352  Total number of prime factors of 3-smooth numbers.
 * https://oeis.org/A069352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069352:  A(n) = i + j for the n‑th 3‑smooth
   number n = 2^i * 3^j.  The value i+j is the total number of
   prime factors (with multiplicity) of that 3‑smooth number. */

#include <stdlib.h>

/* Pre‑computed logarithms of 2 and 3 (double precision). */
#define LOG2 0.6931471805599453094172321214581765680755
#define LOG3 1.0986122886681096913952452369225257046470

seq_t a(idx_t n)
{
    /* Caches for generated exponents and Ω values. */
    static idx_t *exp_i  = NULL;        /* exponent of 2 for each smooth number */
    static idx_t *exp_j  = NULL;        /* exponent of 3 for each smooth number */
    static seq_t *omega  = NULL;        /* Ω = i + j for each smooth number */
    static idx_t   i2 = 0, i3 = 0;       /* indices for generating next 2* and 3* */
    static size_t  sz = 0;              /* number of terms currently stored */
    static size_t  cap = 0;             /* allocated capacity */

    /* n must be >= 1; behaviour for n < 1 is irrelevant. */
    if (n == 0) return 0;

    /* Initialise arrays on first call. */
    if (sz == 0) {
        cap = 16;
        exp_i  = (idx_t *)realloc(exp_i,  cap * sizeof(idx_t));
        exp_j  = (idx_t *)realloc(exp_j,  cap * sizeof(idx_t));
        omega  = (seq_t *)realloc(omega,  cap * sizeof(seq_t));
        exp_i[0] = 0;
        exp_j[0] = 0;
        omega[0] = 0;
        sz = 1;
    }

    /* Grow capacity if necessary. */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        exp_i = (idx_t *)realloc(exp_i,  new_cap * sizeof(idx_t));
        exp_j = (idx_t *)realloc(exp_j,  new_cap * sizeof(idx_t));
        omega = (seq_t *)realloc(omega,  new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Generate terms up to the requested index n. */
    while (sz < n) {
        /* Compute the logarithm (to avoid overflow) of the two candidates. */
        double log2_candidate = (double)(exp_i[i2] + 1) * LOG2 + (double)exp_j[i2] * LOG3;
        double log3_candidate = (double)exp_i[i3] * LOG2 + (double)(exp_j[i3] + 1) * LOG3;

        if (log2_candidate < log3_candidate) {
            exp_i[sz] = exp_i[i2] + 1;
            exp_j[sz] = exp_j[i2];
            omega[sz] = omega[i2] + 1;
            ++i2;
        } else if (log3_candidate < log2_candidate) {
            exp_i[sz] = exp_i[i3];
            exp_j[sz] = exp_j[i3] + 1;
            omega[sz] = omega[i3] + 1;
            ++i3;
        } else { /* log2_candidate == log3_candidate (rare numerical equality) */
            /* Both candidates represent the same number; count once. */
            exp_i[sz] = exp_i[i2] + 1;
            exp_j[sz] = exp_j[i2];
            omega[sz] = omega[i2] + 1;
            ++i2;
            ++i3;
        }
        ++sz;
    }

    return omega[n - 1];
}
