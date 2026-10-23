/*
 * A020912  Number of terms in base 4 representation of n-th Fibonacci number.
 * https://oeis.org/A020912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(log_4(F(n))) + 1, where F(n) is the n‑th Fibonacci
 *            number
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   bitlen_seq_t, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/bitlen_seq_t.h"
#include <stdbool.h>

/* Number of digits in the base‑4 representation of the n‑th Fibonacci number */
seq_t a(idx_t n)
{
    /* The Fibonacci function is guaranteed to be defined for n ≥ 1. */
    seq_t f   = fib(n);                     /* F(n) as a seq_t */
    unsigned bits = bitlen_seq_t(f);        /* number of bits in F(n) */
    /* digits = floor((bits‑1)/2) + 1 */
    seq_t digits = ((seq_t)(bits - 1) >> 1) + 1;
    return digits;
}
