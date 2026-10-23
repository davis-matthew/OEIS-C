/*
 * A155940  Triangle read by rows containing Vardi's optimal solution to the
 *          glove problem.
 * https://oeis.org/A155940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(m,n) = 2 when m = n = 2 a(m,n) = (m+1)/2 when n = 1 and m is
 *            odd a(m,n) = ceil( m/2 + 2*n/3 ) otherwise (The function below
 *            reads the triangle by rows; a(1)=1, a(2)=2, a(3)=2, …)
 * Range:     u8 n <= 1161, u16 n <= 9963, u32 n <= 9963, u64 n <= 9963,
 *            u128 n <= 9963, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static __inline __int128 tri128(idx_t m) {
    return (__int128)m * (m + 1) / 2;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;           /* offset is 1; undefined for n<1 */

    /* Find row m such that m*(m+1)/2 >= n */
    idx_t low = 1, high = 1;
    while (tri128(high) < (__int128)n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (tri128(mid) >= (__int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Column index j (1‑based) */
    idx_t prev = (m > 1) ? (idx_t)tri128(m - 1) : 0;
    idx_t j = n - prev;   /* 1 <= j <= m */

    /* Compute the value using the specification */
    if (m == 2 && j == 2)
        return (seq_t)2;
    if (j == 1 && (m & 1U))
        return ((seq_t)m + 1) / 2;
    /* ceil( (3m + 4j) / 6 )  = (3m + 4j + 5) / 6  */
    seq_t num = (seq_t)3 * m + (seq_t)4 * j + 5;
    return num / 6;
}
