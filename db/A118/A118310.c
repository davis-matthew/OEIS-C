/*
 * A118310  a(n) = gcd(n,m(n)), where m(n) is the n-th nonprime positive
 *          integer (1 or composite).
 * https://oeis.org/A118310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(n, m(n)), where m(n) is the n‑th nonprime (1 or
 *            composite) integer
 * Range:     u8 n <= 2546, u16 n <= 9803, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   gcd, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is 1‑based; value for n==0 is undefined */
        return 0;

    idx_t candidate = 1;
    idx_t count = 0;

    /* search for the n‑th nonprime integer */
    while (true) {
        if (!isPrimeGeneric((seq_t)candidate)) {
            ++count;
            if (count == n) break;
        }
        ++candidate;
    }

    /* compute gcd of n and the found nonprime */
    return gcd((seq_t)n, (seq_t)candidate);
}
