/*
 * A130633  Additive persistence of Fibonacci numbers.
 * https://oeis.org/A130633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A031286(A000045(n)) – additive persistence of the n‑th
 *            Fibonacci number
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 48, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   digit_len, digit_sum, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n);          /* F(n) */
    seq_t p = 0;               /* persistence counter */

    /* Repeatedly sum the decimal digits until a single digit remains */
    while (digit_len(f) > 1) {
        f = digit_sum(f);
        p++;
    }
    return p;
}
