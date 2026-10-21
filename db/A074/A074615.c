/*
 * A074615  a(n) = 5^n + 6^n.
 * https://oeis.org/A074615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 6^n (OEIS A074615)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: exponentiation by squaring for types that support *, integer arithmetic.
 * It works both for built‑in integer types and for arbitrary‑precision seq_t.
 */
static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* Public routine */
seq_t a(idx_t n)
{
    seq_t five  = (seq_t)5;
    seq_t six   = (seq_t)6;
    seq_t p5    = power_seq_t(five, n);
    seq_t p6    = power_seq_t(six,  n);
    return p5 + p6;
}
