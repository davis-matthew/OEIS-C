/*
 * A000959  Lucky numbers.
 * https://oeis.org/A000959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 5983, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve of lucky numbers: start with the odd numbers, then for k = 2, 3, ... delete every a(k)-th remaining number; a(n) is the n-th survivor (computed by sieving) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t max_val = (uint64_t)n * 16;
    if (max_val < 1000) max_val = 1000;

    for (;;) {
        uint64_t cnt = (max_val + 1) / 2;
        uint64_t *lk = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        uint64_t i, k;
        for (i = 0; i < cnt; i++) lk[i] = 2 * i + 1;

        /* sieve with step lk[k] for k = 1, 2, ...; positions < step are unaffected,
           and once step > n the first n positions are final */
        for (k = 1; k < cnt; k++) {
            uint64_t step = lk[k];
            uint64_t w;
            if (step > cnt || step > n) break;
            w = step - 1;
            for (i = step - 1; i < cnt; i++) {
                if ((i + 1) % step != 0) lk[w++] = lk[i];
            }
            cnt = w;
        }

        if (n <= cnt) {
            seq_t r = (seq_t)lk[n - 1];
            free(lk);
            return r;
        }
        free(lk);
        max_val *= 2;
    }
}
