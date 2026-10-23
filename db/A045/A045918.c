/*
 * A045918  Describe n. Also called the "Say What You See" or "Look and Say"
 *          sequence LS(n).
 * https://oeis.org/A045918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the “look‑and‑say” (run‑length encoding) of the decimal *
 *            representation of n. For example, a(23) = 1213
 * Range:     u8 n <= 9, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* digits of n, least significant first */
    unsigned char digits[20];
    size_t len = 0;
    uint64_t tmp = (uint64_t)n;
    do {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    /* compute look‑and‑say */
    seq_t res = (seq_t)0;

    for (int i = (int)len - 1; i >= 0; ) {
        unsigned char cur = digits[i];
        size_t cnt = 1;

        /* count consecutive identical digits */
        while (i - 1 >= 0 && digits[i - 1] == cur) {
            ++cnt;
            --i;
        }

        /* append count */
        unsigned char cnt_digits[20];
        size_t cnt_len = 0;
        size_t ctmp = cnt;
        do {
            cnt_digits[cnt_len++] = (unsigned char)(ctmp % 10);
            ctmp /= 10;
        } while (ctmp > 0);
        for (long long j = (long long)cnt_len - 1; j >= 0; --j) {
            res = res * (seq_t)10 + (seq_t)cnt_digits[j];
        }

        /* append the digit itself */
        res = res * (seq_t)10 + (seq_t)cur;

        /* move to next group */
        --i;
    }

    return res;
}
