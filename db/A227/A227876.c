/*
 * A227876  Write the decimal digits of n and take successive absolute
 *          differences; sequence is the sum of all digits at each level of
 *          the pyramid.
 * https://oeis.org/A227876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, hear
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A227876: sum of all digits in the
   successive absolute‐difference pyramid of the decimal digits of n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 10) return (seq_t)n;          /* single digit case */

    /* maximum decimal digits for a 64‑bit integer is 20, use a safety margin */
    uint8_t tmp[25];
    uint8_t cur[25];
    uint8_t nxt[25];

    idx_t v = n;
    int len = 0;

    /* extract digits (least significant first) */
    while (v > 0) {
        tmp[len++] = v % 10;
        v /= 10;
    }

    /* reverse to most significant first */
    for (int i = 0; i < len; ++i)
        cur[i] = tmp[len - 1 - i];

    seq_t sum = 0;
    for (int i = 0; i < len; ++i)
        sum += (seq_t)cur[i];

    int cur_len = len;
    while (cur_len > 1) {
        for (int i = 0; i < cur_len - 1; ++i) {
            uint8_t diff = (cur[i] > cur[i + 1]) ? (cur[i] - cur[i + 1]) : (cur[i + 1] - cur[i]);
            nxt[i] = diff;
            sum += (seq_t)diff;
        }
        --cur_len;          /* new level has one element less */
        for (int i = 0; i < cur_len; ++i)
            cur[i] = nxt[i];
    }
    return sum;
}
