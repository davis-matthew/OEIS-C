/*
 * A007536  Numbers that are not the sum of 3 hexagonal numbers (probably
 *          finite).
 * https://oeis.org/A007536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, nice
 * Formula:   sieve. Mark every m < 150000 that equals H(i)+H(j)+H(k), where
 *            H(k) = k*(2k-1) and H(0) = 0 is allowed; a(n) is the n-th
 *            unmarked number. The last term is believed to be a(638) = 146858
 * Range:     u8 n <= 86, u16 n <= 635, u32 n <= 638, u64 n <= 638,
 *            u128 n <= 638, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A7536_LIMIT 150000u

seq_t a(idx_t n) {
    static uint32_t *terms = NULL;
    static idx_t nterms = 0;

    if (terms == NULL) {
        unsigned char *rep = (unsigned char *)calloc(A7536_LIMIT, 1);
        uint32_t hex[400];
        idx_t hc = 0;
        uint64_t k, m;
        idx_t i, j, l, cnt;

        for (k = 0; ; k++) {
            uint64_t hk = k * (2 * k) - k;
            if (hk >= A7536_LIMIT) break;
            hex[hc++] = (uint32_t)hk;
        }
        for (i = 0; i < hc; i++) {
            for (j = i; j < hc; j++) {
                uint64_t s2 = (uint64_t)hex[i] + hex[j];
                if (s2 >= A7536_LIMIT) break;
                for (l = j; l < hc; l++) {
                    uint64_t s3 = s2 + hex[l];
                    if (s3 >= A7536_LIMIT) break;
                    rep[s3] = 1;
                }
            }
        }
        cnt = 0;
        for (m = 0; m < A7536_LIMIT; m++)
            if (!rep[m]) cnt++;
        terms = (uint32_t *)malloc((cnt ? cnt : 1) * sizeof(uint32_t));
        for (m = 0; m < A7536_LIMIT; m++)
            if (!rep[m]) terms[nterms++] = (uint32_t)m;
        free(rep);
    }

    if (n < 1 || n > nterms) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
