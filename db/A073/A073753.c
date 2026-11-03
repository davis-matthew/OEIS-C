/*
 * A073753  a(n) = A073752(A073752(n)), where A073752(n) = gcd(n/lpf(n),
 *          n/gpf(n)), with lpf(n) as the least and gpf(n) as the greatest
 *          prime factor of n (see A020639, A006530).
 * https://oeis.org/A073753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A073752(A073752(n)) where A073752(m) = gcd(m/lpf(m),
 *            m/gpf(m)), lpf=least prime factor, gpf=greatest prime factor
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 120 terms through n = 9684 (2026-11-01)
 * Helpers:   gcd, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/largest_prime_factor.h"

/* least prime factor (for n >= 2) */
static uint64_t seq_lpf_uint64(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) return p;
    }
    return n;                     /* n is prime */
}

/* compute A073752 for a 64‑bit value (seq_t may be wider) */
static seq_t seq_a073752_uint64(uint64_t m)
{
    if (m < 2ULL) return (seq_t)1;           /* for m=0 or 1, lpf=gpf=1 */

    uint64_t lpf = seq_lpf_uint64(m);
    uint64_t gpf = largest_prime_factor(m);

    seq_t part1 = (seq_t)m / (seq_t)lpf;      /* m/lpf */
    seq_t part2 = (seq_t)m / (seq_t)gpf;      /* m/gpf */
    return gcd(part1, part2);
}

/* ---------- local helpers ------------------------------------------------- */

/* ---------- main sequence function -------------------------------------- */

seq_t a(idx_t n)
{
    /* first pass */
    seq_t v1 = seq_a073752_uint64((uint64_t)n);

    /* second pass: n >= 1, v1 fits in seq_t, convert to uint64_t safely */
    return seq_a073752_uint64((uint64_t)v1);
}
