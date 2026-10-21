/*
 * A213648  The minimum number of 1's in the relation n*[n,1,1,...,1,n] =
 *          [x,...,x] between simple continued fractions.
 * https://oeis.org/A213648
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n) - 1, where r(n) is the least positive integer k such
 *            that n divides the Fibonacci number F(k). This satisfies the
 *            OEIS conjecture a(n)=A001177(n)-1
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* n >= 2 is assumed by the caller */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Work modulo n.  All Fibonacci terms are computed modulo n.  */
    seq_t mod = (seq_t)n;

    /* Initial Fibonacci values: F(1)=1, F(2)=1 */
    seq_t prev = 1 % mod;    /* F(k-2)  */
    seq_t curr = 1 % mod;    /* F(k-1)  */
    idx_t k = 2;             /* current index (curr == F(k)) */

    while (1) {
        /* Compute next = (prev + curr) mod mod,
           taking care with unsigned overflow.  */
        seq_t next = prev + curr;

        /* Detect unsigned overflow: if next < prev after addition */
        if (next < prev) {
            /* One wrap-around occurred; subtract one modulus.  */
            next -= mod;
        }
        /* In the unlikely case that subtraction still leaves next >= mod
           (possible when mod is close to UINTMAX), wrap once more.  */
        if (next >= mod) {
            next -= mod;
        }

        ++k;  /* we are now at F(k) */

        if (next == (seq_t)0) {
            /* r(n) = k, answer = r(n) - 1.  */
            return (seq_t)(k - (idx_t)1);
        }

        /* Advance for next iteration */
        prev = curr;
        curr = next;
    }
}
