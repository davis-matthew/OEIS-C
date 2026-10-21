/*
 * A229040  Fibonacci numbers in which parity of the decimal digits
 *          alternates.
 * https://oeis.org/A229040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth Fibonacci number whose decimal digits alternate
 *            parity (odd-even-odd-even ...). This implementation generates
 *            Fibonacci numbers sequentially, tests each one for the
 *            alternating parity property using the helper get_digits, and
 *            returns the nth qualifying value. Duplicate Fibonacci v...
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"

/* Return true if the decimal digits of x alternate in parity. */
static bool has_alternating_parity(seq_t x)
{
    /* 0 is considered alternating (single digit). */
    if (x == 0)
        return true;

    uint8_t digits[5000];   /* Buffer large enough for Fibonacci numbers up to F(19000). */
    size_t len;
    get_digits(x, digits, &len);   /* digits[0] = most significant digit */

    for (size_t i = 1; i < len; ++i) {
        if ((digits[i - 1] & 1u) == (digits[i] & 1u))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;        /* how many qualifying Fibonacci numbers seen */
    seq_t f_prev = 0;       /* current Fibonacci number F(k)   */
    seq_t f_curr = 1;       /* next Fibonacci number F(k+1)   */
    seq_t prev_fib = 0;     /* last Fibonacci checked (to skip duplicates) */
    bool first_iter = true; /* true for the very first iteration */

    /* Iterate indefinitely until the nth qualifying Fibonacci is found. */
    while (1) {
        /* Only consider a Fibonacci value once (skip the duplicate 1). */
        if (first_iter || f_prev != prev_fib) {
            if (has_alternating_parity(f_prev)) {
                ++count;
                if (count == n)
                    return f_prev;
            }
            prev_fib = f_prev;
            first_iter = false;
        }

        /* Advance to the next Fibonacci number. */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
