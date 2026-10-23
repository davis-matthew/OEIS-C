/*
 * A131676  a(n) = (Product_{i=1..6} n^i+i) / 6!.
 * https://oeis.org/A131676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∏_{i=1..6} (n^i + i)) / 6!
 * Range:     u8 none, u16 n <= 2, u32 n <= 3, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t denom = (seq_t)720;           /* 6! */

    /* For 8, 16, 32 or 64‑bit seq_t use a 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        unsigned __int128 prod = 1;
        unsigned __int128 pow  = 1;

        for (idx_t i = 1; i <= 6; ++i)
        {
            pow  *= (unsigned __int128)n;          /* pow = n^i */
            prod *= (pow + (unsigned __int128)i);   /* prod *= (n^i + i) */
        }

        prod /= (unsigned __int128)denom;           /* divide by 720 */
        return (seq_t)prod;                        /* cast back to seq_t */
    }
    else
    {
        seq_t prod = 1;
        seq_t pow  = 1;

        for (idx_t i = 1; i <= 6; ++i)
        {
            pow  = pow * (seq_t)n;                  /* pow = n^i */
            prod = prod * (pow + (seq_t)i);          /* prod *= (n^i + i) */
        }

        prod = prod / denom;                        /* divide by 720 */
        return prod;                                /* result in seq_t */
    }
}
