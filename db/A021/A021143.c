/*
 * A021143  Decimal expansion of 1/139.
 * https://oeis.org/A021143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( (10 * (10^n mod 139)) / 139 ) Recurrence from
 *            OEIS: a(n) = a(n-1) - a(n-23) + a(n-24) for n > 23
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = 10^n (mod 139) using fast exponentiation. */
    idx_t exp = n;
    unsigned long long result = 1ULL;   /* r   */
    unsigned long long base  = 10ULL;   /* 10^1 */

    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % 139ULL;
        base = (base * base) % 139ULL;
        exp >>= 1ULL;
    }

    /* The nth decimal digit is floor(10 * r / 139).  r < 139, so no overflow. */
    unsigned long long digit = (result * 10ULL) / 139ULL;
    return (seq_t)digit;
}
