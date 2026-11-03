/*
 * A140730  a(4*n)=5^n, a(4*n+1)=2*5^n, a(4*n+2)=3*5^n, a(4*n+3)=4*5^n.
 * https://oeis.org/A140730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1-4*floor(n/4))*5^floor(n/4)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t k = n / 4;          /* floor(n/4) */
    unsigned r = n % 4;       /* n mod 4 */
    seq_t p = power5(k);      /* 5^floor(n/4) */
    return ((seq_t)(r + 1)) * p;
}
