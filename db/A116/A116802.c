/*
 * A116802  Number of permutations of length n which avoid the patterns 1342,
 *          3421, 4213.
 * https://oeis.org/A116802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 40*a(n-2) + 82*a(n-3) - 92*a(n-4) + 56*a(n-5)
 *            - 17*a(n-6) + 2*a(n-7) for n ≥ 8, with a(1)=1, a(2)=2, a(3)=6,
 *            a(4)=21, a(5)=73, a(6)=244, a(7)=787
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle requests below the offset */
    if (n == 0) return (seq_t)0;

    /* First seven terms are known explicitly */
    if (n <= (idx_t)7) {
        static const seq_t init[7] = {
            (seq_t)1,  (seq_t)2,  (seq_t)6,
            (seq_t)21, (seq_t)73, (seq_t)244,
            (seq_t)787
        };
        return init[(size_t)(n - (idx_t)1)];
    }

    /* sliding window for the 7‑term linear recurrence */
    seq_t prev[7] = {
        (seq_t)787, /* a(7) */
        (seq_t)244, /* a(6) */
        (seq_t)73,  /* a(5) */
        (seq_t)21,  /* a(4) */
        (seq_t)6,   /* a(3) */
        (seq_t)2,   /* a(2) */
        (seq_t)1    /* a(1) */
    };

    seq_t val;
    for (idx_t k = (idx_t)8; k <= n; ++k) {
        val = (seq_t)0;
        val += prev[0] * (seq_t)10;   /* 10*a(k-1)   */
        val += prev[2] * (seq_t)82;   /* 82*a(k-3)   */
        val += prev[4] * (seq_t)56;   /* 56*a(k-5)   */
        val += prev[6] * (seq_t)2;    /* 2*a(k-7)    */
        val -= prev[1] * (seq_t)40;   /* 40*a(k-2)   */
        val -= prev[3] * (seq_t)92;   /* 92*a(k-4)   */
        val -= prev[5] * (seq_t)17;   /* 17*a(k-6)   */

        /* shift window for next iteration */
        for (int i = 6; i >= 1; --i)
            prev[i] = prev[i - 1];
        prev[0] = val;
    }

    return prev[0];
}
