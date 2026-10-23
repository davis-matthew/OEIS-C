/*
 * A023785  Katadromes: digits in base 3 are in strict descending order.
 * https://oeis.org/A023785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   A finite set of numbers whose base‑3 representation has strictly
 *            decreasing digits: 0, 1, 2, 3 (10₃), 6 (20₃), 7 (21₃), 21 (210₃)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t seq[] = {0, 1, 2, 3, 6, 7, 21};
    size_t len = sizeof(seq) / sizeof(seq[0]);

    if (n == 0) return 0;                         /* offset starts at 1 */
    if (n <= (idx_t)len) return seq[(size_t)(n - 1)];
    return 0;                                      /* undefined for larger n */
}
