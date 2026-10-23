/*
 * A172264  a(n) = floor(n*(sqrt(3)-sqrt(2))).
 * https://oeis.org/A172264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(3)-sqrt(2)))
 * Range:     u8 n <= 805, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if m <= floor(n*(sqrt(3)-sqrt(2))) */
static inline bool cond(idx_t n, idx_t m) {
    __int128 nm = n;
    __int128 mm = m;
    __int128 lhs = 8 * mm * mm * nm * nm;          /* 8·m²·n²                           */
    __int128 diff = nm * nm - mm * mm;             /* n² - m²                           */
    __int128 rhs = diff * diff;                     /* (n² - m²)²                        */
    return lhs <= rhs;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t low = 0, high = n - 1; /* a(n) < n for α<1 */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (cond(n, mid))
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}
