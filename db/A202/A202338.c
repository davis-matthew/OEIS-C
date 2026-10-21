/*
 * A202338  Smallest Fibonacci number which when appended to n produces a
 *          prime number.
 * https://oeis.org/A202338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 77, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the smallest Fibonacci number f such
 * that the decimal concatenation of n and f is prime.
 * It generates Fibonacci numbers iteratively and checks each candidate
 * by constructing the concatenated number and using the provided
 * is_prime_big routine. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/pow10.h"           /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"       /* int digit_len(seq_t x); */

seq_t a(idx_t n)
{
    /* start with the first two Fibonacci numbers 1,1 */
    seq_t f_prev = 1;   /* F(1) */
    seq_t f_curr = 1;   /* F(2) */

    /* test the very first Fibonacci number (1) */
    seq_t pow_w = pow10((idx_t)digit_len(f_prev));
    seq_t concat = pow_w * (seq_t)n + f_prev;
    if (is_prime_big(concat))
        return f_prev;

    /* iterate over subsequent Fibonacci numbers */
    while (true) {
        /* check current Fibonacci number */
        pow_w = pow10((idx_t)digit_len(f_curr));
        concat = pow_w * (seq_t)n + f_curr;
        if (is_prime_big(concat))
            return f_curr;

        /* compute next Fibonacci number */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
