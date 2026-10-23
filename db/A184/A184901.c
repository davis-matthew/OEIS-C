/*
 * A184901  n+floor(ns/r)+floor(nt/r), where r=(1+sqrt(5))/2, s=r+1, t=r+2.
 * https://oeis.org/A184901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+floor(ns/r)+floor(nt/r), where r=(1+sqrt(5))/2, s=r+1,
 *            t=r+2
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(v)) */
static seq_t isqrt_seq(seq_t v) {
    seq_t low  = 0;
    seq_t high = v;
    while (low <= high) {
        seq_t mid  = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1; for n == 0 we return 0. */
    if (n == 0) return 0;

    seq_t N = (seq_t)n;                     /* n as seq_t */
    seq_t p = isqrt_seq((seq_t)5 * N * N);  /* floor(n * sqrt(5)) */
    seq_t x = N + p;                         /* n + floor(n*sqrt5)   */
    seq_t c = x / 2;                         /* floor((n + p) / 2)   */
    return N + c + p;                       /* a(n) = n + c + p     */
}
