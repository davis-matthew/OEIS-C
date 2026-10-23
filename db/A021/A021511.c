/*
 * A021511  Decimal expansion of 1/507.
 * https://oeis.org/A021511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( (10 * (10^n mod 507)) / 507 ) Sequence A021511:
 *            decimal expansion of 1/507, a(0) is the first digit after the
 *            decimal point. Implemented with modular exponentiation (O(log
 *            n))
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 10^n (mod 507) using binary exponentiation. */
    unsigned long long result = 1ULL % 507ULL;      /* current 10^k mod 507 */
    unsigned long long base   = 10ULL % 507ULL;     /* 10^(2^i) mod 507 */
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % 507ULL;
        base = (base * base) % 507ULL;
        exp >>= 1;
    }

    /* digit = floor( (10 * result) / 507 ).  result < 507, so no overflow. */
    unsigned long long digit = (result * 10ULL) / 507ULL;
    return (seq_t)digit;
}
