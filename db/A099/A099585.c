/*
 * A099585  Remove all factors of 3 from prime(n) - 1.
 * https://oeis.org/A099585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (prime(n)-1) / 3^v3(prime(n)-1)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute a(1)=1, a(2)=2, a(3)=4, a(4)=2, … */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime.  n is 1‑based. */
    seq_t p = (seq_t)get_prime(n);
    seq_t x = p - 1;              /* p-1 is always ≥1 */

    /* Remove all factors of 3. */
    while ((x % (seq_t)3) == 0) {
        x /= (seq_t)3;
    }
    return x;
}
