/*
 * A209629  The number of partitions of the set [n] where each element can be
 *          colored 1 or 2 avoiding the patterns 1^12^2 and 1^22^1 in the
 *          pattern sense.
 * https://oeis.org/A209629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + 2*(B(n)-1), where B(n) is the Bell number
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Cached array of Bell numbers; bell[0] = 1. */
static seq_t *bell = NULL;          /* dynamic array of Bell numbers */
static idx_t  bell_size = 0;        /* number of computed entries  (bell_size = max index + 1) */

/* Allocate or enlarge the bell array to hold indices up to 'max_index' inclusive. */
static void ensure_bell_capacity(idx_t max_index)
{
    if (bell_size > max_index)
        return;                       /* already enough */

    /* Allocate new array of size max_index + 1. */
    size_t new_len = (size_t)(max_index + 1);
    seq_t *tmp = (seq_t *)realloc(bell, new_len * sizeof(seq_t));
    if (!tmp)                /* memory exhausted – abort */
        exit(EXIT_FAILURE);
    bell = tmp;

    /* Compute new Bell numbers, starting after the last computed one. */
    for (idx_t m = bell_size; m <= max_index; ++m) {
        seq_t next = (seq_t)0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t binom = binomial(m - 1, k);   /* C(m-1,k) */
            seq_t prod  = bell[k] * binom;      /* multiplication works for both
                                                    built‑in and arbitrary‑precision
                                                    types */
            next += prod;
        }
        bell[m] = next;
    }
    bell_size = new_len;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)                /* not used by OEIS, but harmless */
        return (seq_t)0;

    /* Initialise Bell cache if necessary. */
    if (bell == NULL) {
        bell = (seq_t *)calloc(1, sizeof(seq_t));   /* B(0) = 1 */
        if (!bell)
            exit(EXIT_FAILURE);
        bell[0] = (seq_t)1;
        bell_size = 1;
    }

    /* Ensure Bell numbers up to n are computed. */
    ensure_bell_capacity(n);

    /* Compute 2^n + 2*(B(n)-1) safely. */
    seq_t two_power_n = pow2(n);
    seq_t two_times_B_minus_1 = (bell[n] - (seq_t)1) * ((seq_t)2);
    return two_power_n + two_times_B_minus_1;
}
