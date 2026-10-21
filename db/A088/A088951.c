/*
 * A088951  Number of distinct square-subwords in ternary representation of n.
 * https://oeis.org/A088951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct square-subwords in the ternary
 *            representation of n
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Convert n to base‑3 string */
    uint8_t digits[64];   /* digits in reverse order, most significant first */
    size_t len = 0;
    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t t = n;
        while (t > 0) {
            digits[len++] = t % 3;
            t /= 3;
        }
        /* reverse to most significant first */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t tmp = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = tmp;
        }
    }

    /* Temporary buffer for a candidate square (null‑terminated) */
    char cur[64];
    /* Storage for distinct squares, up to a few dozen */
    char uniq[64][64];
    size_t uniq_cnt = 0;

    for (size_t i = 0; i < len; ++i) {
        for (size_t half = 1; i + 2 * half <= len; ++half) {
            /* Check whether substring digits[i..i+2*half-1] is a square */
            size_t j;
            for (j = 0; j < half; ++j)
                if (digits[i + j] != digits[i + half + j])
                    break;
            if (j == half) {
                /* Build character string for this square */
                for (size_t k = i; k < i + 2 * half; ++k)
                    cur[k - i] = '0' + digits[k];
                cur[2 * half] = '\0';

                /* Test uniqueness */
                bool found = false;
                for (size_t u = 0; u < uniq_cnt; ++u)
                    if (strcmp(uniq[u], cur) == 0) {
                        found = true;
                        break;
                    }
                if (!found) {
                    strcpy(uniq[uniq_cnt], cur);
                    ++uniq_cnt;
                }
            }
        }
    }
    return (seq_t)uniq_cnt;
}
