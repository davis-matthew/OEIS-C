/*
 * A132958  a(n) = n!*Sum_{d|n} (-1)^(d+1)/d!.
 * https://oeis.org/A132958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*Sum_{d|n} (-1)^(d+1)/d!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

/* Computes the OEIS sequence A132958 */
seq_t a(idx_t n)
{
    /* Compute n! once */
    seq_t fact_n = factorial(n);

    seq_t pos = 0;   /* sum of positive terms */
    seq_t neg = 0;   /* sum of negative terms */

    /* Enumerate divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* First divisor d */
        seq_t fact_d = factorial(d);
        seq_t ratio  = fact_n / fact_d;
        if (d & 1)      /* d is odd -> (+) */
            pos += ratio;
        else            /* d is even -> (-) */
            neg += ratio;

        /* Second divisor n/d if distinct */
        idx_t d2 = n / d;
        if (d2 == d)
            continue;
        seq_t fact_d2 = factorial(d2);
        seq_t ratio2  = fact_n / fact_d2;
        if (d2 & 1)
            pos += ratio2;
        else
            neg += ratio2;
    }

    /* The final result is pos - neg (guaranteed non‑negative) */
    return pos - neg;
}
