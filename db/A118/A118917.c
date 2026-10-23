/*
 * A118917  Number of inequivalent primes in ring of integers Z[sqrt(2)] with
 *          absolute value of norm = n.
 * https://oeis.org/A118917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9695, u16 n <= 9695, u32 n <= 9695, u64 n <= 9695,
 *            u128 n <= 9695, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118917
   a(n) = 2 if n is prime ≡ 1,7 (mod 8);
   a(n) = 1 if n = 2 or n = p² where p is prime ≡ 3,5 (mod 8);
   a(n) = 0 otherwise.  */

#include <stdint.h>
#include <stdbool.h>

/* --------- helper functions ------------------------------------- */

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* floor of sqrt for unsigned 64‑bit integers, no overflow */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > n / mid) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

/* --------- main sequence function ------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* a(0) = a(1) = 0 */

    if (n == 2) return (seq_t)1;   /* a(2) = 1 */

    /* prime check and residue mod 8 */
    if (is_prime_u64(n) && ((n % 8) == 1 || (n % 8) == 7))
        return (seq_t)2;

    /* square of a prime with residue mod 8 = 3 or 5 */
    uint64_t r = isqrt_u64(n);
    if (r * r == n && is_prime_u64(r) &&
        ((r % 8) == 3 || (r % 8) == 5))
        return (seq_t)1;

    return (seq_t)0;
}
