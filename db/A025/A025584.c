/*
 * A025584  Primes p such that p-2 is not a prime.
 * https://oeis.org/A025584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n‑th prime p such that p = 2 or p = 3 or (p−2 is not
 *            prime). The function generates primes sequentially and tests the
 *            required condition. This faithfully follows the OEIS definition
 *            and works for any seq_t, idx_t combination
 * Range:     u8 n <= 37, u16 n <= 5682, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    idx_t count = 0;
    idx_t p = 2;           /* start from the first prime */

    for (;; p++)
    {
        if (!is_prime(p))
            continue;

        bool ok;
        if (p == 2 || p == 3)
            ok = true;
        else
            ok = !is_prime(p - 2);

        if (ok)
        {
            count++;
            if (count == n)
                return (seq_t)p;   /* cast to seq_t for the result */
        }
    }
}
