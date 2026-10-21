/*
 * A093222  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 22.
 * https://oeis.org/A093222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=0; a(2)=88; For n≥3: • if n is odd: a(n)= [9 repeated
 *            (n‑1)] 0 [2 repeated (n‑1)] 0 • if n is even: a(n)= [8 repeated
 *            (n‑2)] 66 [8 repeated (n‑2)] 6 (All numbers are interpreted in
 *            base‑10.)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* For any helper types; not used directly */

seq_t a(idx_t n)
{
    /* Handle the small, special cases first */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)88;

    seq_t val = (seq_t)0;

    if (n & 1U) {                 /* n is odd  (n ≥ 3) */
        idx_t k = n - 1;          /* Number of 9’s and 2’s */

        /* Leading block of 9's */
        for (idx_t i = 0; i < k; ++i) {
            val = val * (seq_t)10 + (seq_t)9;
        }

        /* The following 0 */
        val = val * (seq_t)10;    /* add digit 0 */

        /* Block of 2's */
        for (idx_t i = 0; i < k; ++i) {
            val = val * (seq_t)10 + (seq_t)2;
        }

        /* final trailing 0 */
        val = val * (seq_t)10;
    } else {                      /* n is even  (n ≥ 4) */
        idx_t k = n - 2;          /* Number of 8’s in each side */

        /* Leading block of 8's */
        for (idx_t i = 0; i < k; ++i) {
            val = val * (seq_t)10 + (seq_t)8;
        }

        /* The middle 66 */
        val = val * (seq_t)10 + (seq_t)6;
        val = val * (seq_t)10 + (seq_t)6;

        /* Trailing block of 8's */
        for (idx_t i = 0; i < k; ++i) {
            val = val * (seq_t)10 + (seq_t)8;
        }

        /* Final trailing 6 */
        val = val * (seq_t)10 + (seq_t)6;
    }

    return val;
}
