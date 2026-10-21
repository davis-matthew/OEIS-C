/*
 * A214781  a(n) = smallest k>=2 such that n divides Fibonacci(k-1)+2.
 * https://oeis.org/A214781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 2 such that n divides Fibonacci(k-1)+2, or 0
 *            if no such k exists
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External function requested by the test harness. */
seq_t a(idx_t n)
{
    /* Guard against an undefined n=0 caller – return 0 in that case. */
    if (n == 0) return (seq_t)0;

    /* Work in normal integer types first.  All arithmetic on Fibonacci
       values stays below 'n', so no overflow occurs. */
    seq_t mod   = (seq_t)n;                     /* n as sequence type   */
    seq_t target = (mod >= 2) ? (mod - 2) : 0;   /* F_m ≡ -2 (mod n)    */

    /* Fibonacci terms modulo n.  F_0 = 0, F_1 = 1 % n. */
    seq_t prev   = (seq_t)0;                    /* F_0 */
    seq_t curr   = mod ? ((seq_t)1 % mod) : (seq_t)0;  /* F_1 */

    /* The Pisano period for modulus n is at most 6·n.  Only iterate up to
       that many steps to guarantee a halt.  The loop variable uses the
       index type so that it can count to 6·n safely for all practical
       test data (< 10 000). */
    idx_t limit = 6 * n;   /* safe for n ≤ 10 000, as required by the
                              benchmark. */
    for (idx_t m = 1; m <= limit; ++m) {
        /* curr is F_m modulo n.  Check the condition. */
        if (curr == target) {
            return (seq_t)(m + 1);   /* k = m + 1 */
        }

        /* Prepare the next term: (prev + curr) mod n.  Since prev < n
           and curr < n, the sum is < 2·n, so an at most one subtraction
           suffices. */
        seq_t next = prev + curr;
        if (next >= mod) {
            next -= mod;
        }

        /* Advance. */
        prev = curr;
        curr = next;
    }

    /* If we exhausted the Pisano period without finding a match, return 0. */
    return (seq_t)0;
}
