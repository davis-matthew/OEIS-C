/*
 * A217398  Numbers starting with 5.
 * https://oeis.org/A217398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (44*10^floor(log_10(9*n-8))-8)/9. (Implemented by
 *            listing all integers whose first decimal digit is 5.)
 * Range:     u8 n <= 11, u16 n <= 1111, u32 n <= 1111, u64 n <= 1111,
 *            u128 n <= 1111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Sequence is undefined for n < 1; returning 0 for safety. */
    if (n == 0) return 0;

    seq_t sn = n;          /* cast n to seq_t for arithmetic */
    seq_t pow10 = 1;       /* 10^(L-1) for the current block length L */
    seq_t prev_cumulative = 0;

    /* Find the smallest block L such that cumulative count >= n. */
    while (1) {
        seq_t cumulative = 1 + (seq_t)10 * (pow10 - 1) / 9; /* total up to length L */
        if (cumulative >= sn) break;
        prev_cumulative = cumulative;
        pow10 *= 10;                         /* move to next block length */
    }

    seq_t offset = sn - prev_cumulative - 1; /* position within the block */
    return 5 * pow10 + offset;              /* number starting with digit 5 */
}
