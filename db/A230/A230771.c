/*
 * A230771  Union of Fibonacci numbers and powers of 2.
 * https://oeis.org/A230771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   fib, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Union of Fibonacci numbers and powers of two
 * Formula: a(n) = nth element of the sorted union of Fibonacci numbers
 *           and numbers of the form 2^k, without duplicates.
 */
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1, so 0 is invalid */
        return (seq_t)0;

    /* Current indices / values for Fibonacci and powers of two */
    idx_t fib_idx   = 1;          /* F₁ = 1 */
    idx_t pow_idx   = 0;          /* 2⁰ = 1 */
    seq_t cur_fib   = fib(fib_idx);
    seq_t cur_pow   = (seq_t)1;   /* 2⁰ */

    idx_t count = 0;
    seq_t result = 0;

    while (true) {
        seq_t next_val;
        bool use_fib = false;
        bool use_pow = false;

        /* Choose the smaller (or equal) current value */
        if (cur_fib < cur_pow) {
            next_val = cur_fib;
            use_fib  = true;
        } else if (cur_pow < cur_fib) {
            next_val = cur_pow;
            use_pow  = true;
        } else { /* equal */
            next_val = cur_fib;   /* same as cur_pow */
            use_fib  = use_pow = true;
        }

        ++count;
        if (count == n) {
            result = next_val;
            break;
        }

        /* Advance the Fibonacci generator if it contributed to this value */
        if (use_fib) {
            do {
                ++fib_idx;
                cur_fib = fib(fib_idx);
            } while (cur_fib == next_val);   /* skip duplicates of 1, etc. */
        }

        /* Advance the powers-of-two generator if it contributed */
        if (use_pow) {
            do {
                ++pow_idx;
                cur_pow = mul_small(cur_pow, 2);   /* 2^pow_idx */
            } while (cur_pow == next_val);   /* skip duplicates like 2,8,… */
        }
    }

    return result;
}
