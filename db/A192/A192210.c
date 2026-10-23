/*
 * A192210  a(n) = sum of unsigned coefficients in (1+x+x^2-x^3)^n.
 * https://oeis.org/A192210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of absolute values of the coefficients of
 *            (1+x+x^2-x^3)^n This implementation builds the two unsigned
 *            polynomials A and B representing even/odd parity contributions,
 *            then computes the L1 norm
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *sum_cache = NULL;          /* cache of a(n) */
static size_t cache_cap = 0;
static idx_t cached_max = 0;             /* highest n that has been computed */

static seq_t *A_prev = NULL;             /* coefficients of even-parity part  */
static seq_t *B_prev = NULL;             /* coefficients of odd-parity part   */
static size_t prev_len = 0;              /* length of current coefficient arrays */

/* Ensure sum_cache has capacity at least 'needed' */
static void grow_cache(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = needed;
    if (new_cap < 8) new_cap = 8;
    while (new_cap < needed) new_cap = new_cap + new_cap / 2;
    sum_cache = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise for n = 0 */
static void init_first(void) {
    sum_cache = (seq_t *)malloc(sizeof(seq_t));
    sum_cache[0] = (seq_t)1;
    cache_cap = 1;
    cached_max = 0;

    prev_len = 1;
    A_prev = (seq_t *)malloc(sizeof(seq_t));
    B_prev = (seq_t *)malloc(sizeof(seq_t));
    A_prev[0] = (seq_t)1;
    B_prev[0] = (seq_t)0;
}

/* Compute values up to index n */
static void compute_until(idx_t n) {
    if (sum_cache == NULL) init_first();

    grow_cache((size_t)n + 1);

    for (idx_t cur = cached_max + 1; cur <= n; ++cur) {
        size_t new_len = 3 * cur + 1;
        seq_t *newA = (seq_t *)calloc(new_len, sizeof(seq_t));
        seq_t *newB = (seq_t *)calloc(new_len, sizeof(seq_t));

        for (size_t j = 0; j < prev_len; ++j) {
            seq_t a = A_prev[j];
            if (a != (seq_t)0) {
                newA[j]     += a;
                newA[j + 1] += a;
                newA[j + 2] += a;
                newB[j + 3] += a;
            }
            seq_t b = B_prev[j];
            if (b != (seq_t)0) {
                newB[j]     += b;
                newB[j + 1] += b;
                newB[j + 2] += b;
                newA[j + 3] += b;
            }
        }

        /* sum of absolute differences */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < new_len; ++i) {
            seq_t va = newA[i];
            seq_t vb = newB[i];
            if (va >= vb)
                total += va - vb;
            else
                total += vb - va;
        }
        sum_cache[cur] = total;
        cached_max = cur;

        free(A_prev);
        free(B_prev);
        A_prev = newA;
        B_prev = newB;
        prev_len = new_len;
    }
}

seq_t a(idx_t n) {
    compute_until(n);
    return sum_cache[n];
}
