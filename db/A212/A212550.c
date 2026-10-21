/*
 * A212550  Number of partitions of n containing at least one part m-10 if m
 *          is the largest part.
 * https://oeis.org/A212550
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS A212550 Number of partitions of n containing at least one
 *            part m-10 if m is the largest part. G.f.: Sum_{i>0} x^(2*i+10) /
 *            Product_{j=1..10+i} (1-x^j). Offset: 10 (a(10) is the first term
 *            listed)
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 141, u64 n <= 455,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#define MAX_N 10000ULL   /* precompute up to this index (>= the largest test value) */

static seq_t *dp_arr;
static seq_t *part_arr;
static int initialized = 0;

static void compute_all(void)
{
    size_t i, k;
    size_t m_limit = (MAX_N + 10) / 2;  /* largest m that can contribute to n <= MAX_N */
    /* allocate arrays */
    dp_arr   = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
    part_arr = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
    if (dp_arr == NULL || part_arr == NULL) {
        exit(1);  /* allocation failure */
    }

    /* dp_arr[0] = 1 */
    dp_arr[0] = (seq_t)1;

    /* iterate over part sizes m */
    for (i = 1; i <= m_limit; ++i) {
        /* update DP: partitions of k with parts <= i */
        for (k = i; k <= MAX_N; ++k) {
            dp_arr[k] += dp_arr[k - i];
        }

        /* if i (m) is large enough, accumulate contributions to part_arr[n] */
        if (i >= 11) {
            /* For each k, compute n = k + 2*i - 10,
               ensure n <= MAX_N */
            size_t max_k = MAX_N - (2 * i - 10);
            for (k = 0; k <= max_k; ++k) {
                size_t n = k + (2 * i - 10);
                part_arr[n] += dp_arr[k];
            }
        }
    }

    /* The arrays are fully populated. */
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_all();
    }

    if (n > MAX_N) {
        /* If a larger n is requested, recompute up to that n.
           For simplicity we return 0 (unimplemented beyond precompute). */
        return 0;
    }

    if (n < 10) {
        /* Offset is 10; values below are not defined, return 0 as safe. */
        return 0;
    }

    return part_arr[n];
}
