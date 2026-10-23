/*
 * A034049  Numbers with multiplicative digital root value 2.
 * https://oeis.org/A034049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 2291, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative digital root of a number is found by repeated
   multiplication of its decimal digits.  a(n) is the n‑th positive
   integer whose multiplicative digital root equals 2. */
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t num = 2; ; ++num) {
        seq_t x = (seq_t)num;
        while (x >= (seq_t)10)          /* repeatedly multiply digits */
            x = digit_product(x);
        if (x == (seq_t)2) {             /* root 2 → candidate */
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}
