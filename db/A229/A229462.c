/*
 * A229462  Idoneal numbers (A000926) that are not pentagon exception numbers
 *          (A229461).
 * https://oeis.org/A229462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 7 * 4^((n-1)/2) for odd n, a(n) = 15 * 4^((n/2)-1) for
 *            even n
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t pow4(idx_t e) {
    seq_t res = 1;
    for (idx_t i = 0; i < e; ++i) res *= 4;
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1; undefined for n<1 */
    if (n & 1) { /* odd n */
        idx_t k = (n - 1) / 2;
        return 7 * pow4(k);
    } else { /* even n */
        idx_t k = n / 2;
        return 15 * pow4(k - 1);
    }
}
