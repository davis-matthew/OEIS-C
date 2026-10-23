/*
 * A178757  Smallest k such that k*n has an odd number of 1's in its base-2
 *          expansion.
 * https://oeis.org/A178757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A178757: a(n) = smallest odd k such that
   the binary representation of k*n contains an odd number of 1's
   (i.e. popcount(k*n) is odd).  The function assumes (for all
   supported build types) that product k*n fits into seq_t; for
   typical 64‑bit or 128‑bit builds this holds for the requested range
   of n. */
#include <stdint.h>
#include <stdbool.h>

/* helper: returns the parity of the popcount of x (0 for even,
   1 for odd).  Works for any unsigned integral type used for seq_t. */
static inline unsigned int popcount_parity(seq_t x)
{
    unsigned int parity = 0;
    while (x)
    {
        parity ^= 1;   /* flip parity on each 1-bit */
        x &= x - 1;    /* clear lowest set bit */
    }
    return parity;
}

/* OEIS sequence A178757 */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; n==0 is undefined,
                            return 0 as a safe fallback */
        return (seq_t)0;

    const seq_t N = (seq_t)n; /* cast index to seq_t for multiplication */

    for (seq_t k = 1; ; k += 2)   /* only odd k are needed */
    {
        seq_t prod = k * N;      /* product may use 128-bit if seq_t is so */

        if (popcount_parity(prod) == 1)
            return k;            /* found the smallest odd k */
    }
}
