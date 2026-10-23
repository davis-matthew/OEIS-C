/*
 * A162015  Numbers n with property that the sum of the digits of n is
 *          substring of n and of n^2.
 * https://oeis.org/A162015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 495, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   digit_len, digit_sum, seq_mul, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162015: numbers n such that sum of digits of n is a substring of n and of n². */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"
#include "../utils/seq_to_str.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 1;

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)last_checked;

        /* digit sum of cand */
        seq_t sum = digit_sum(cand);

        /* sum as string */
        size_t sumlen = digit_len(sum);
        char *sumstr = (char *)malloc(sumlen + 1);
        size_t slen_out;
        seq_to_str(sum, sumstr, &slen_out);
        sumstr[slen_out] = '\0';

        /* cand as string */
        size_t candlen = digit_len(cand);
        char *candstr = (char *)malloc(candlen + 1);
        size_t clen_out;
        seq_to_str(cand, candstr, &clen_out);
        candstr[clen_out] = '\0';

        /* square of cand */
        seq_t sq = seq_mul(cand, cand);
        size_t sqlen = digit_len(sq);
        char *sqstr = (char *)malloc(sqlen + 1);
        size_t sqout;
        seq_to_str(sq, sqstr, &sqout);
        sqstr[sqout] = '\0';

        bool ok = (strstr(candstr, sumstr) != NULL) &&
                  (strstr(sqstr, sumstr) != NULL);

        free(sumstr);
        free(candstr);
        free(sqstr);

        if (ok) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }

        last_checked++;
    }

    return cache[n - 1];
}
