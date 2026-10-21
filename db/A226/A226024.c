/*
 * A226024  Integers k such that (sum of digits of 10234*k) = k.
 * https://oeis.org/A226024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for the sequence A226024.
   Computes all integers k with digit_sum(10234*k) == k,
   stores them in a static array, and returns the nth element
   (1‑based indexing). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

static const unsigned long long MULT = 10234ULL;

seq_t a(idx_t n)
{
    /* ----- lazy initialisation of all terms ----- */
    static seq_t *solutions = NULL;
    static size_t sol_cnt = 0;

    if (!solutions) {
        /* Allocate room for at most 1000 potential solutions. */
        solutions = (seq_t *)malloc(sizeof(seq_t) * 1000);
        /* Scan k up to 1000 – all solutions are < 54. */
        for (idx_t k = 1; k <= 1000 && sol_cnt < 1000; ++k) {
            seq_t prod = (seq_t)k * (seq_t)MULT;
            if (digit_sum(prod) == k) {
                solutions[sol_cnt++] = k;
            }
        }
    }

    if (n == 0 || n > sol_cnt)
        return 0;             /* out of range – no further terms */

    return solutions[n - 1];
}
