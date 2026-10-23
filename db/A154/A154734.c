/*
 * A154734  Define k(0) = 2 and k(m) = m^2-k(m-1) for m >= 1. This is a list
 *          of those terms k(m) for which k(m)+1 and k(m)-1 are both in
 *          A008578 (primes including 1).
 * https://oeis.org/A154734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 10, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: k(0)=2; k(m)=m^2‑k(m-1).
 * The sequence a(n) consists of those k(m) for which k(m)‑1 and
 * k(m)+1 are both prime (with 1 treated as prime).  a(1)=k(0). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Handle the first term immediately. */
    if (n == 1)
        return (seq_t)2;

    /* Use a signed 64‑bit integer for intermediate k values.
     * Only the recurrence may produce a negative value (k(1) = –1),
     * and that happens only once before k becomes positive again.
     * k is always small enough to fit in int64_t for the limits
     * we consider (n ≤ 10000). */
    int64_t k = 2;   /* k(0) */
    idx_t   m = 0;   /* current index in the recurrence */
    idx_t   count = 1; /* how many qualifying terms have been seen */

    while (true)
    {
        /* Next index m >= 1. */
        ++m;

        /* m^2 may overflow 64 bits for astronomically large m,
         * but for n ≤ 10000 the required m will be far smaller,
         * so this is safe. */
        int64_t m_sq = (int64_t)m * (int64_t)m;

        /* Compute k(m) = m^2 - k(m-1). */
        k = m_sq - k;

        /* Check the primality of k-1 and k+1.
         * For a value equal to 1 we consider it prime. */
        int64_t p1 = k + 1;
        int64_t p2 = k - 1;
        bool prime1 = (p1 == 1) || (p1 >= 2 && is_prime_big((seq_t)p1));
        bool prime2 = (p2 == 1) || (p2 >= 2 && is_prime_big((seq_t)p2));

        if (prime1 && prime2)
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
