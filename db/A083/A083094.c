/*
 * A083094  Numbers k such that Sum_{j=0..k} (binomial(k,j) mod 3) is odd.
 * https://oeis.org/A083094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth smallest non‑negative integer whose base‑3
 *            representation - uses only the digits 0 and 2, - contains an
 *            even number of 2’s. These integers are exactly the numbers k
 *            such that Sum_{j=0..k} (binomial(k,j) mod 3) is odd
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(1) = 0.  All subsequent terms come from generating numbers
   whose base‑3 digits are 0/2 with even parity of 2’s.  The
   construction enumerates them in increasing order.  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* The function is required under the name a.  The types seq_t and
   idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* a(1) == 0 */
    if (n == 1) return (seq_t)0;

    /* Count includes the initial zero */
    idx_t count = 1;

    /* Generate numbers with base‑3 digits 0 or 2.
       For length L (number of digits in base 3),
       the leading digit must be 2.
       The remaining L-1 digits are encoded by a bit mask of size L-1.
       We require the total number of 2 digits (i.e., 1's in the mask
       plus the leading 2) to be even.  This is equivalent to the
       lower L-1 bits having an odd popcount.  */
    for (int L = 2;
         ; ++L)   /* length increases until we find the nth term */
    {
        /* Number of possible masks for this length */
        unsigned long long maxMask = 1ULL << (L - 1);
        for (unsigned long long mask = 1ULL; mask < maxMask; ++mask)
        {
            /* Only consider masks with odd popcount */
            if ((__builtin_popcountll(mask) & 1ULL) == 0ULL)
                continue;

            /* Build the decimal value from the base‑3 digits.
               Start with the leading 2 (most significant digit).  */
            seq_t val = (seq_t)2;
            for (int pos = L - 2; pos >= 0; --pos)
            {
                val = val * (seq_t)3;
                if (mask & (1ULL << pos))
                    val += (seq_t)2;
            }

            ++count;          /* This val is the next term a(count) */
            if (count == n)
                return val;
        }
    }
}
