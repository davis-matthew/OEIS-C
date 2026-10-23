/*
 * A164825  Fibonacci numbers in which the difference between the largest
 *          digit and the smallest digit is a prime.
 * https://oeis.org/A164825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   is_prime_digit_diff, ok_fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Fibonacci numbers where the difference between the largest
 * and smallest decimal digit is a prime (2, 3, 5, or 7).  The function
 * a(n) returns the n‑th such Fibonacci number (offset 1).  No external
 * state is stored; the sequence is generated on demand. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_digit_diff.h"
#include "../utils/ok_fib.h"

/* Main routine: returns the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Fibonacci initialization: f_prev = F(k-1), f_curr = F(k) */
    seq_t f_prev = (seq_t)0;
    seq_t f_curr = (seq_t)1;
    idx_t found  = 0;          /* number of terms found so far */

    /* Iterate Fibonacci numbers until the n-th valid term is found */
    while (1) {
        /* Compute next Fibonacci number */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;

        /* Test the current Fibonacci number */
        if (ok_fib(f_curr)) {
            ++found;
            if (found == n) {
                return f_curr;
            }
        }
    }
}
