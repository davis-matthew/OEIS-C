/*
 * A214594  Number of n X n X n triangular 0..7 arrays with every horizontal
 *          row having the same average value.
 * https://oeis.org/A214594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×n×n triangular arrays with entries 0..7 such that
 *            each row has the same arithmetic mean. For each average c in
 *            0..7 the rows are independent; the number of ways for row i
 *            equals the number of compositions of i*c into i parts each
 *            between 0 and 7. That number is computed by inclusion–e...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utilities from the evaluation harness */
#include "../utils/binomial.h"

/* Return the number of solutions of
     x1 + … + xi = s   with   0 ≤ xj ≤ 7.
   i and s are non‑negative integers (idx_t).
   The result is type seq_t. */
static seq_t ways_row(idx_t i, idx_t s)
{
    idx_t maxk = s / 8;           /* floor(s/8) – largest k for inclusion–exclusion */
    seq_t pos = 0;                /* sum of terms with even k */
    seq_t neg = 0;                /* sum of terms with odd  k */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term = binomial(i, k);
        term = term * binomial(s - 8 * k + i - 1, i - 1);

        if ((k & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    /* The true count is non‑negative; pos ≥ neg in every case. */
    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n == 0) /* not defined for n < 1, but return 0 safely */
        return 0;

    seq_t total = 0;

    /* For each possible average c (0 … 7) accumulate the product
       of the number of ways for every row i = 1 … n. */
    for (idx_t c = 0; c <= 7; ++c)
    {
        seq_t prod = 1;          /* product over rows for this c */

        for (idx_t i = 1; i <= n; ++i)
        {
            idx_t sum = i * c;        /* desired row sum for row i */
            seq_t ways = ways_row(i, sum);
            prod *= ways;
        }
        total += prod;
    }

    return total;
}
