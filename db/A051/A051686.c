/*
 * A051686  Smallest prime p such that 2n*p+1 is also prime.
 * https://oeis.org/A051686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime p such that 2*n*p + 1 is also prime (OEIS A051686). */
#include <stdint.h>
#include <stdbool.h>

/* Simple primality test for 64‑bit values.  Works for all candidates
   produced by this sequence (2*n*p+1 never exceeds 2^64 for the tested
   range of n). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
    {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0; harmless fallback */

    uint64_t un = (uint64_t)n;       /* convert index to 64‑bit for computation   */
    uint64_t p  = 2;                 /* start with the smallest prime           */

    for (;;)
    {
        uint64_t candidate = 2ULL * un * p + 1ULL;  /* 2*n*p + 1          */
        if (is_prime_uint64(candidate))
            return (seq_t)p;       /* cast back to seq_t (fits in any unsigned type) */

        /* Advance to the next prime.  Simple increment‑and‑test loop. */
        do { p++; } while (!is_prime_uint64(p));
    }
}
