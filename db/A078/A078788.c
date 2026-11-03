/*
 * A078788  a(n) is the smallest m > 2 such that (1 - n) mod m = 0, or 0 if no
 *          such number exists.
 * https://oeis.org/A078788
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime factor of (n-1), or 0 if (n-1) has no
 *            * such factor (i.e., if n-1 is a power of 2). This implements
 *            the definition * of OEIS sequence A078788
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 89 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t smallest_odd_prime_factor(uint64_t v)
{
    if (v <= 2)              /* 1 or 2 → no odd prime divisor */
        return 0;
    /* Remove all factors of 2 */
    while ((v & 1ULL) == 0)  /* v % 2 == 0 */
        v >>= 1;             /* v /= 2 */
    if (v == 1)              /* only factor 2 remained */
        return 0;

    /* Find the smallest odd divisor of the remaining odd part */
    for (uint64_t d = 3; d * d <= v; d += 2) {
        if (v % d == 0)
            return d;        /* first odd divisor found is the smallest odd prime factor */
    }
    return v;                /* v itself is prime and odd */
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;     /* offset; behaviour for n < 2 is unspecified */

    uint64_t x = (uint64_t)(n - 1);
    uint64_t p = smallest_odd_prime_factor(x);
    return (seq_t)p;
}
