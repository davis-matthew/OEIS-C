/*
 * A052010  Smallest k such that the sum of the digits of n each raised to
 *          k-th power is greater than n, or 0 if no such k exists.
 * https://oeis.org/A052010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k such that sum of the digits of n each raised
 *            to k-th power is greater than n; 0 if no such k exists
 * Range:     u8 n <= 79, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 0: single digit 0, never exceeds itself */
    if (n == 0)
        return 0;

    /* Extract decimal digits of n into dig[]. */
    uint8_t dig[20];
    size_t len = 0;
    idx_t t = n;
    do {
        dig[len++] = (uint8_t)(t % 10);
        t /= 10;
    } while (t);

    /* If n contains no digit >=2, no k will work. */
    bool has_ge2 = false;
    for (size_t i = 0; i < len; ++i) {
        if (dig[i] >= 2) {
            has_ge2 = true;
            break;
        }
    }
    if (!has_ge2)
        return 0;

    /* cur[d] holds d^k for digit d. Initial k = 1. */
    seq_t cur[10];
    cur[0] = 0;
    cur[1] = 1;
    for (int d = 2; d <= 9; ++d)
        cur[d] = (seq_t)d;

    const seq_t nseq = (seq_t)n;

    for (idx_t k = 1;; ++k) {
        seq_t sum = 0;
        for (size_t i = 0; i < len; ++i) {
            uint8_t d = dig[i];
            seq_t val = (d == 0) ? 0 : ((d == 1) ? 1 : cur[d]);

            /* If this digit alone already exceeds n, the sum will exceed n. */
            if (val > nseq)
                return (seq_t)k;

            /* Avoid overflow when adding val to sum. */
            if (sum > nseq - val)
                return (seq_t)k;
            sum += val;
        }
        if (sum > nseq)
            return (seq_t)k;

        /* Prepare powers for next k. */
        for (int d = 2; d <= 9; ++d) {
            if (cur[d] > nseq) /* already large; keep it large. */
                continue;
            cur[d] *= (seq_t)d;
        }
    }
}
