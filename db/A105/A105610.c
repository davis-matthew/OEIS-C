/*
 * A105610  Numbers k such that both p1=2k+3 and p2=4k+5 are primes.
 * https://oeis.org/A105610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth non‑negative integer k such that 2*k+3 and 4*k+5
 *            are primes
 * Range:     u8 n <= 20, u16 n <= 1445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence starts with a(1) = 0, a(2) = 2, a(3) = 8, ... */
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, but return 0 safely */

    idx_t    count = 0;            /* how many k's found so far */
    uint64_t k     = 0;            /* current candidate k */

    while (count < n) {
        uint64_t p1 = 2ULL * k + 3ULL;
        if (!is_prime(p1)) {        /* first condition failed, advance */
            ++k;
            continue;
        }
        uint64_t p2 = 4ULL * k + 5ULL;
        if (is_prime(p2)) {
            /* this k is the next term */
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)k;
}
