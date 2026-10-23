/*
 * A175186  a(1)=0. For 1<= n <= 2^m, m>=0, a(n+ 2^m) = a(n)+n.
 * https://oeis.org/A175186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>=2 let p=2^⌊log2(n-1)⌋, then
   a(n)=a(n-p)+(n-p).  */
#include <stdint.h>

/* compute floor(log2(v)) for an unsigned integer v */
static inline idx_t floor_log2_idx(idx_t v) {
    idx_t m = 0;
    idx_t t = v;
    while (t >>= 1) ++m;
    return m;
}

seq_t a(idx_t n) {
    if (n == 1) return 0;
    seq_t sum = 0;
    idx_t cur = n;
    while (cur > 1) {
        idx_t m = floor_log2_idx(cur - 1);
        idx_t p = (idx_t)(1ULL << m);  /* highest power of two <= cur-1 */
        idx_t r = cur - p;             /* remainder */
        sum += (seq_t)r;
        cur = r;
    }
    return sum;
}
