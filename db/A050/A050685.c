/*
 * A050685  Number of nonzero palindromes < 10^n and containing at least one
 *          digit '0'.
 * https://oeis.org/A050685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 19*a(n-2) – 19*a(n-3) – 90*a(n-4) + 90*a(n-5)
 * with initial values
 *   a(1) = 0, a(2) = 0, a(3) = 9, a(4) = 18, a(5) = 189
 * and a(0) is defined as 0 for safety.
 *
 * This implements the recurrence in O(n) time and O(1) extra space,
 * handling any seq_t type that supports +, -, and seq_mul(seq_t,size_t).
 */

#include <stdint.h>
#include "../utils/seq_mul.h"   /* provides seq_mul(seq_t a, unsigned int m) */

seq_t a(idx_t n)
{
    /* handle the offset and trivial cases directly */
    if (n == 0) return (seq_t)0;
    if (n <= 5) {
        static const seq_t init[6] = {
            (seq_t)0, /* unused: init[0] = 0 */
            (seq_t)0, /* a(1) */
            (seq_t)0, /* a(2) */
            (seq_t)9, /* a(3) */
            (seq_t)18,/* a(4) */
            (seq_t)189/* a(5) */
        };
        return init[n];
    }

    /* sliding window: a1…a5 store the last five terms
     * a1  = a(i-5)
     * a2  = a(i-4)
     * a3  = a(i-3)
     * a4  = a(i-2)
     * a5  = a(i-1)    */
    seq_t a1 = (seq_t)0;   /* a(1)   */
    seq_t a2 = (seq_t)0;   /* a(2)   */
    seq_t a3 = (seq_t)9;   /* a(3)   */
    seq_t a4 = (seq_t)18;  /* a(4)   */
    seq_t a5 = (seq_t)189; /* a(5)   */
    seq_t an;

    for (idx_t i = 6; i <= n; ++i) {
        /* recurrence: a(i) = a5 + 19*a4 – 19*a3 – 90*a2 + 90*a1 */
        seq_t tmp = a5;
        tmp = tmp + seq_mul(a4, 19);
        tmp = tmp - seq_mul(a3, 19);
        tmp = tmp - seq_mul(a2, 90);
        tmp = tmp + seq_mul(a1, 90);
        an = tmp;

        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}
