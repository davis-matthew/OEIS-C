/*
 * A081749  Continued fraction for e/5.
 * https://oeis.org/A081749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   for k ≥ 1, a(19k) = a(19k+15) = a(19k+16) = a(19k+18) = 3;
 *            a(19k+1) = a(19k+6) = 2k; a(19k+2) = a(19k+5) = a(19k+7) =
 *            a(19k+10) = a(19k+13) = 1; a(19k+3) = a(19k+4) = a(19k+8) =
 *            a(19k+9) = 2; a(19k+11)= a(19k+14) = 2k+1; a(19k+17) = 2k+2; For
 *            0 ≤ n < 19 the terms are the explicit initial values:...
 * Range:     u8 n <= 2429, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */

static const seq_t base_terms[19] = {
    0, 1, 1, 5, 4, 2, 2, 2, 2, 2,
    1, 1, 9, 1, 1, 3, 3, 2, 3
};

seq_t a(idx_t n)
{
    if (n < 19U) {
        return base_terms[(size_t)n];
    }

    idx_t k = n / 19U;          /* k ≥ 1 for n ≥ 19 */
    unsigned j = (unsigned)(n % 19U);

    seq_t kk = (seq_t)k;        /* cast for subsequent arithmetic */

    switch (j) {
    case 0:
    case 15:
    case 16:
    case 18:
        return 3;               /* constant 3 */

    case 1:
    case 6:
        return (seq_t)2 * kk;   /* 2k */

    case 2:
    case 5:
    case 7:
    case 10:
    case 13:
        return 1;               /* constant 1 */

    case 3:
    case 4:
    case 8:
    case 9:
        return 2;               /* constant 2 */

    case 11:
    case 14:
        return (seq_t)2 * kk + 1;/* 2k+1 */

    case 12:
        return 9;               /* constant 9 */

    case 17:
        return (seq_t)2 * kk + 2;/* 2k+2 */

    default:
        return 0;               /* unreachable, safety net */
    }
}
