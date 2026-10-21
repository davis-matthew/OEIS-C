/*
 * A083822  a(n) = digit reversal of 3*n, divided by 3.
 * https://oeis.org/A083822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = reverse(3*n)/3
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes 3*n, reverses its decimal digits, then divides by 3 (exact division) */

#include <stdint.h>

/* Helper to reverse decimal digits of an unsigned seq_t number */
static inline seq_t reverse_seq_t(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t digit = x % 10;   /* extract last decimal digit */
        rev = rev * 10 + digit; /* append it to the reversed number */
        x = x / 10;             /* drop processed digit */
    }
    return rev;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t m   = (seq_t)n * 3;      /* 3 * n */
    seq_t rev = reverse_seq_t(m);   /* reverse decimal digits of 3*n */
    return rev / 3;                /* exact division by 3 */
}
