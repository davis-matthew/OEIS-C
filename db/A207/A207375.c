/*
 * A207375  Irregular array read by rows in which row n lists the (one or two)
 *          central divisors of n in increasing order.
 * https://oeis.org/A207375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 497, u16 n <= 9930, u32 n <= 9930, u64 n <= 9930,
 *            u128 n <= 9930, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A207375 – central divisors of integers read row‑wise.
 *   The array has rows:
 *    row r contains the square root of r if r is a square;
 *    otherwise it contains the pair of divisors (d, r/d) with d ≤ r/d
 *    closest to sqrt(r).  
 *   The sequence a(n) is the n‑th element of the flattened rows.
 *
 * Implementation: scan rows until the n‑th element is reached,
 * then return the appropriate divisor.  All arithmetic is done
 * using idx_t (uint64_t) and seq_t (the specified result type).
 */

#include <stdint.h>

/* Integer floor square root for an unsigned 64‑bit integer.
 *  Uses a simple loop that avoids overflow by comparing with n/(i+1). */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t i = 0;
    while ((i + 1) <= n / (i + 1))
        ++i;
    return i;
}

/* a(n) – nth term of the A207375 sequence. */
seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t r          = 1;                 /* current row number */

    for (;; ++r)
    {
        /* Length of current row: 1 if r is a perfect square, else 2. */
        idx_t sq = floor_sqrt_u64(r);
        idx_t row_len = (sq * sq == r) ? 1 : 2;
        cumulative += row_len;

        if (cumulative >= n)              /* the desired element is in row r */
        {
            idx_t offset = n - (cumulative - row_len);  /* 1‑based position in row */

            if (row_len == 1)              /* r is a square */
            {
                return (seq_t)sq;          /* the single divisor is sqrt(r) */
            }
            else /* row_len == 2, r not a square */
            {
                /* Find the largest divisor d ≤ sqrt(r). */
                idx_t d = sq;
                while (r % d != 0)
                    --d;

                if (offset == 1)
                    return (seq_t)d;        /* smaller central divisor */
                else
                    return (seq_t)(r / d);  /* larger central divisor */
            }
        }
    }

    /* Unreachable, but keeps the compiler happy. */
    return (seq_t)0;
}
