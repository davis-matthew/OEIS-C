/*
 * A173524  a(n) = A053737(4^k+n-1) in the limit k->infinity, where k plays
 *          the role of a row index in A053737.
 * https://oeis.org/A173524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum of base‑4 digits of (n-1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the nth term of sequence A173524.
   idx_t n (1‑based).   seq_t is an unsigned integer type of any width. */
seq_t a(idx_t n) {
    /* n ≥ 1; behaviour for n == 0 is undefined (offset 1). */
    idx_t m = n - 1;          /* base‑4 exponent */
    seq_t sum = 0;
    while (m > 0) {
        sum += (seq_t)(m & 3); /* add least significant base‑4 digit */
        m >>= 2;              /* shift right by 2 bits (divide by 4) */
    }
    return sum + 1;           /* add 1 as per formula */
}
