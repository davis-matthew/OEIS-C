/*
 * A001283  Triangle read by rows, in which row n consists of n(n+m) for m = 1
 *          .. n-1.
 * https://oeis.org/A001283
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(k, m) = k*(k+m), where n = 2 + (k-2)*(k-1)/2 + (m-1), 1
 *            <= m <= k-1
 * Range:     u8 n <= 65, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k = 2;      /* current row */
    idx_t start = 2;  /* index of first entry of row k */
    idx_t m;

    if (n < 2) {
        return 0;
    }
    /* row k has k-1 entries */
    while (n >= start + (k - 1)) {
        start += k - 1;
        k++;
    }
    m = n - start + 1;
    return (seq_t)k * ((seq_t)k + (seq_t)m);
}
