/*
 * A189313  Primes p such that 2*omega(4p) = omega(4p+2), where omega(k) is
 *          the number of distinct primes dividing k, A001221.
 * https://oeis.org/A189313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 2 * ω(4p) = ω(4p+2); For p > 2
 *            this is equivalent to ω(2p+1) = 3; p = 2 is a special solution.
 *            The code computes the terms by brute‑force testing of successive
 *            primes
 * Range:     u8 n <= 7, u16 n <= 1100, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   count_distinct_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"          /* is_prime(idx_t) */
#include "../utils/count_distinct_primes.h"  /* count_distinct_primes(uint64_t) */

/* Return the n-th term (n ≥ 1).  Undefined behaviour for n == 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* optional safe fallback */

    idx_t found = 0;

    /* 2 is the first solution. */
    if (n == 1)
        return (seq_t)2;

    /* Scan odd numbers greater than 2. */
    for (idx_t p = 3; ; p += 2) {
        if (!is_prime(p))
            continue;

        /* Compute v = 2*p + 1.  Use 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 v128 = (unsigned __int128)p * 2ULL + 1ULL;

        /* Overflow beyond 64 bits would mean we can no longer use
           count_distinct_primes(uint64_t).  For the intended n (≤ 10 000),
           this never happens. */
        if (v128 > (unsigned __int128)UINT64_MAX)
            break;

        uint64_t v = (uint64_t)v128;

        /* For p>2, need ω(v) == 3. */
        if (count_distinct_primes(v) == 3) {
            ++found;
            if (found + 1 == n)    /* +1 for the initial 2 */
                return (seq_t)p;
        }
    }

    /* Not reached for valid n within bounds. */
    return (seq_t)0;
}
