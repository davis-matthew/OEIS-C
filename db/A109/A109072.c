/*
 * A109072  Number of decimal digits (counted with multiplicity) in n but not
 *          in n^2.
 * https://oeis.org/A109072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of decimal digits in n that do not occur in n^2
 *            (digits counted with multiplicity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9895 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count decimal digits of a 128‑bit integer */
static void count_digits_uint128(unsigned __int128 v, unsigned char cnt[10]) {
    if (v == 0) {
        cnt[0] = 1;
        return;
    }
    while (v) {
        cnt[(int)(v % 10)] ++;
        v /= 10;
    }
}

/* Count decimal digits of a 64‑bit integer */
static void count_digits_uint64(uint64_t v, unsigned char cnt[10]) {
    if (v == 0) {
        cnt[0] = 1;
        return;
    }
    while (v) {
        cnt[v % 10] ++;
        v /= 10;
    }
}

seq_t a(idx_t n) {
    unsigned char cnt_n[10] = {0};
    unsigned char cnt_n2[10] = {0};
    unsigned char *p;
    int d;

    /* Count digits of n */
    count_digits_uint64((uint64_t)n, cnt_n);

    /* Count digits of n^2 (128‑bit) */
    unsigned __int128 n2 = (unsigned __int128)n * (unsigned __int128)n;
    count_digits_uint128(n2, cnt_n2);

    /* Count those digits of n that never appear in n^2 */
    seq_t res = 0;
    for (d = 0; d < 10; ++d) {
        if (cnt_n[d] && cnt_n2[d] == 0) {
            res += cnt_n[d];
        }
    }
    return res;
}
