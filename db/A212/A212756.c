/*
 * A212756  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          2w-x=max{w,x,y,z}-min{w,x,y,z}.
 * https://oeis.org/A212756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212756
 * Recurrence (order 13):
 *   a(0) = 1,  a(1) = 4,  a(2) = 17,  a(3) = 32,  a(4) = 71,  a(5) = 104,
 *   a(6) = 187, a(7) = 247, a(8) = 380, a(9) = 484, a(10)=685, a(11)=823,
 *   a(12)=1122
 *   for n ≥ 13:
 *   a(n) = -2*a(n-1)+5*a(n-3)+7*a(n-4)-9*a(n-6)-9*a(n-7)+7*a(n-9)+5*a(n-10)
 *          -2*a(n-12)-a(n-13)
 *
 * This implementation computes the term iteratively using a circular buffer
 * of the last 13 values.  No memoisation or recursion is required.
 */

#include <stddef.h>   /* for size_t */

static const seq_t base_vals[13] = {
    (seq_t)1,    /* a(0) */
    (seq_t)4,    /* a(1) */
    (seq_t)17,   /* a(2) */
    (seq_t)32,   /* a(3) */
    (seq_t)71,   /* a(4) */
    (seq_t)104,  /* a(5) */
    (seq_t)187,  /* a(6) */
    (seq_t)247,  /* a(7) */
    (seq_t)380,  /* a(8) */
    (seq_t)484,  /* a(9) */
    (seq_t)685,  /* a(10) */
    (seq_t)823,  /* a(11) */
    (seq_t)1122  /* a(12) */
};

seq_t a(idx_t n) {
    if (n < 13) {
        return base_vals[(size_t)n];
    }

    /* circular buffer containing the last 13 values a(k-13 .. k-1)   */
    seq_t buf[13];
    for (size_t i = 0; i < 13; ++i) {
        buf[i] = base_vals[i];
    }

    /* compute a(k) for k = 13 .. n */
    for (idx_t k = 13; k <= n; ++k) {
        seq_t val =  (seq_t)5 * buf[(k - 3) % 13]  /* +5*a(k-3)   */
                     + (seq_t)7 * buf[(k - 4) % 13]  /* +7*a(k-4)   */
                     + (seq_t)7 * buf[(k - 9) % 13]  /* +7*a(k-9)   */
                     + (seq_t)5 * buf[(k -10) % 13]  /* +5*a(k-10)  */
                     - (seq_t)2 * buf[(k - 1) % 13]  /* -2*a(k-1)   */
                     - (seq_t)9 * buf[(k - 6) % 13]  /* -9*a(k-6)   */
                     - (seq_t)9 * buf[(k - 7) % 13]  /* -9*a(k-7)   */
                     - (seq_t)2 * buf[(k -12) % 13]  /* -2*a(k-12)  */
                     -        buf[(k -13) % 13];    /* -a(k-13)    */

        buf[k % 13] = val;
    }

    return buf[n % 13];
}
