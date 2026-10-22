/*
 * A006920  At each step, record how many 1's, 2's, etc. have been seen so far
 *          in the sequence.
 * https://oeis.org/A006920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   computed by simulating the sequence as a list of numbers. a(1) =
 *            1. For n >= 2, a(n) is the decimal concatenation of c(1), c(2),
 *            ..., c(m), where c(k) is how many times k occurs among the
 *            numbers written so far and m is the largest of those numbers.
 *            Zero counts are written as "0". The numbers a(n...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n) {
    size_t cap = 64;
    uint64_t *cnt = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t m = 1;
    seq_t r = 0;
    idx_t i;

    if (n <= 1) {
        free(cnt);
        free(buf);
        return (seq_t)1;
    }
    cnt[1] = 1;

    for (i = 2; i <= n; i++) {
        size_t len = (size_t)m;
        size_t j;
        for (j = 0; j < len; j++) buf[j] = cnt[j + 1];

        if (i == n) {
            for (j = 0; j < len; j++) {
                uint64_t e = buf[j], t = e;
                seq_t p = 1;
                do {
                    p *= 10;
                    t /= 10;
                } while (t > 0);
                r = r * p + (seq_t)e;
            }
            break;
        }

        for (j = 0; j < len; j++) {
            uint64_t e = buf[j];
            if (e >= cap) {
                size_t nc = cap;
                while (nc <= e) nc *= 2;
                cnt = (uint64_t *)realloc(cnt, nc * sizeof(uint64_t));
                memset(cnt + cap, 0, (nc - cap) * sizeof(uint64_t));
                buf = (uint64_t *)realloc(buf, nc * sizeof(uint64_t));
                cap = nc;
            }
            if (e > m) m = e;
            cnt[e]++;
        }
    }

    free(cnt);
    free(buf);
    return r;
}
