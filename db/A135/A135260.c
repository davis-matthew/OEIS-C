/*
 * A135260  Fibonacci Connell sequence: 1 odd, 1 even, 2 odd, 3 even, 5 odd, 8
 *          even, ....
 * https://oeis.org/A135260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A135260 – Fibonacci Connell sequence
   Blocks of increasing size follow the Fibonacci numbers,
   alternating odd and even blocks.  Start from 1.
   a(n) is the term at index n (offset 1). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1; return 0 as fallback */
        return (seq_t)0;

    idx_t cumulative = 0;    /* terms counted so far */
    seq_t last = 0;         /* last value produced */

    /* Fibonacci numbers for block sizes */
    idx_t fib_prev1 = 1;     /* f(1) */
    idx_t fib_prev2 = 1;     /* f(2) */

    for (idx_t block = 1; ; ++block) {
        idx_t block_size;
        if (block == 1 || block == 2) {
            block_size = 1;
        } else {
            idx_t next = fib_prev1 + fib_prev2;
            block_size = next;
            fib_prev2 = fib_prev1;
            fib_prev1 = next;
        }

        if (cumulative + block_size >= n) {
            /* target lies in this block */
            idx_t offset = n - cumulative - 1;          /* 0‑based inside block */
            int desired_parity = block & 1;             /* 1 for odd block, 0 for even */

            seq_t start;
            if ((last & 1) == desired_parity) {
                start = last + 2;
            } else {
                start = last + 1;
            }

            return start + (seq_t)(2 * offset);
        }

        /* skip entire block */
        int desired_parity = block & 1;
        seq_t start;
        if ((last & 1) == desired_parity) {
            start = last + 2;
        } else {
            start = last + 1;
        }

        last = start + (seq_t)(2 * (block_size - 1));
        cumulative += block_size;
    }
}
