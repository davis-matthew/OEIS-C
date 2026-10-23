/*
 * A125258  Smallest prime divisor of n^4-n^2+1.
 * https://oeis.org/A125258
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime divisor of n^4 - n^2 + 1, for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Trial division to find the smallest prime factor of a 64‑bit number.
   Assumes n ≥ 2 and returns n if n is prime. */
static uint64_t _spf_uint64(uint64_t n)
{
    if ((n & 1ULL) == 0ULL)
        return 2ULL;
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL)
        if (n % p == 0ULL)
            return p;
    return n;   /* n is prime */
}

seq_t a(idx_t n)
{
    if (n < 2ull)
        return 0;   /* offset is 2; below that is undefined */

    /* Compute n^4 - n^2 + 1 using 128‑bit arithmetic to avoid overflow
       for all n that fit in 64‑bit (which includes the test range). */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 expr128 = nn * nn * nn * nn - nn * nn + 1;

    /* If the value fits in 64 bits, use the small‑prime factor routine.
       For larger values we fall back to a trial division on the 128‑bit
       number itself (still safe for the expected input size). */
    if (expr128 <= (unsigned __int128)ULLONG_MAX) {
        uint64_t expr64 = (uint64_t)expr128;
        return (seq_t)_spf_uint64(expr64);
    } else {
        /* For values > 2^64, perform trial division on the __int128 value.
           The loop uses 64‑bit primes; division is cheap for the test
           range and the smallest divisor will be found quickly. */
        if ((expr128 & 1) == 0)
            return (seq_t)2;
        for (uint64_t p = 3; (unsigned __int128)p * p <= expr128; p += 2) {
            if ((unsigned __int128)expr128 % p == 0)
                return (seq_t)p;
        }
        /* If no divisor found, expr128 itself is prime. */
        return (seq_t)expr128;
    }
}
