/*
 * A087243  a(n) = n + A087242(n) or a(n)=0 if A087242(n)=0; the primes
 *          arising as n + A087242(n).
 * https://oeis.org/A087243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + min{ x prime | n + x is prime } or 0 if no such prime
 *            exists. * For an odd n only x = 2 can work; for an even n we
 *            scan odd primes starting * at 3 until n + p is prime. The
 *            routine uses a simple deterministic ______
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic primality test for n <= 2⁶⁴‑1.
 *  It works for the range needed to test the sequence up to the limits used in
 *  the test harness (n ≤ 10000, so n + p ≤ 20000).  For larger values the test
 *  remains correct as long as the product i*i does not overflow 64 bits.
 */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)               return false;
    if (x % 2 == 0)          return x == 2;
    for (uint64_t i = 3; i <= x / i; i += 2) {   /* use x/i to avoid overflow */
        if (x % i == 0)      return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n is >= 1 (OEIS offset 1) */
    uint64_t un = (uint64_t)n;

    /* Odd n: only 2 can be added.  Return n+2 if that sum is prime, else 0. */
    if (un & 1ULL) {
        if (is_prime_uint64(un + 2ULL))
            return (seq_t)(un + 2ULL);
        else
            return (seq_t)0;
    }

    /* Even n: search odd primes p starting at 3 until n + p is prime. */
    for (uint64_t p = 3; ; p += 2) {
        if (!is_prime_uint64(p))
            continue;
        if (is_prime_uint64(un + p))
            return (seq_t)(un + p);
        /* In principle this loop is infinite if no suitable prime exists.
         * The sequence is defined only for indices where such a prime occurs,
         * but for the bounded values used here it always terminates.        */
    }
}
