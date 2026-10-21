/*
 * A088224  Numbers that fill in the gaps between primes in a list of twin
 *          primes.
 * https://oeis.org/A088224
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer ≥ 4 that is **not** a twin‑prime prime;
 *            it lists the gaps between consecutive twin‑prime endpoints.
 *            a(3)=4, a(4)=6, a(5)=8, a(6)=9, a(7)=10, …
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3. */
    if (n < 3)
        return (seq_t)0;

    /* Target index within the sequence (0‑based).  a(3) → index 0. */
    idx_t target = n - 3;
    idx_t count  = 0;

    for (idx_t num = 4;; ++num)
    {
        bool is_twin = false;

        if (is_prime(num))
        {
            /* test whether num has a prime partner at distance 2 */
            if ((num > 2 && is_prime(num - 2)) || is_prime(num + 2))
                is_twin = true;
        }

        if (!is_twin)          /* num is part of the sequence */
        {
            if (count == target)
                return (seq_t)num;

            ++count;
        }
    }
}
