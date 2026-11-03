/*
 * A063850  Say what you see in previous term, reporting total number for each
 *          digit encountered.
 * https://oeis.org/A063850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0) = 1; for n > 0, a(n) is obtained by reading the previous
 *            term a(n‑1), counting how many times each digit 0‑9 appears
 *            (processing the distinct digits in order of first appearance),
 *            and writing each count followed by the digit
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* core computation of the next term from the previous */
static seq_t next_term(seq_t prev)
{
    /* extract digits of prev (most significant first) */
    unsigned char digits[32];      /* enough for 64‑bit values */
    int len = 0;
    if (prev == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        seq_t t = prev;
        while (t > 0) {
            digits[len++] = (unsigned char)(t % 10);
            t /= 10;
        }
        /* reverse to obtain MSB‑first order */
        for (int i = 0; i < len / 2; i++) {
            unsigned char tmp = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = tmp;
        }
    }

    /* count occurrences of each digit */
    unsigned int cnt[10] = {0};
    for (int i = 0; i < len; i++) cnt[digits[i]]++;

    /* build the next term */
    seq_t result = 0;
    unsigned char seen[10] = {0};
    for (int i = 0; i < len; i++) {
        unsigned char d = digits[i];
        if (!seen[d]) {
            seen[d] = 1;
            /* digit count */
            unsigned int c = cnt[d];
            unsigned char cDigits[10];
            int cLen = 0;
            if (c == 0) { /* never happens but keep for completeness */
                cDigits[0] = 0;
                cLen = 1;
            } else {
                while (c > 0) {
                    cDigits[cLen++] = (unsigned char)(c % 10);
                    c /= 10;
                }
                /* reverse to MSB‑first */
                for (int j = 0; j < cLen / 2; j++) {
                    unsigned char tmp = cDigits[j];
                    cDigits[j] = cDigits[cLen - 1 - j];
                    cDigits[cLen - 1 - j] = tmp;
                }
            }
            /* append count digits */
            for (int j = 0; j < cLen; j++) {
                result = result * 10 + cDigits[j];
            }
            /* append the digit itself */
            result = result * 10 + d;
        }
    }
    return result;
}

/* memoised evaluation of a(n) */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    if (n < cached_len) return cache[n];

    /* extend cache to size n+1 */
    idx_t new_len = n + 1;
    cache = mem_resize(seq_t, cache, new_len);
    if (!cache) {
        /* allocation failure – abort */
        return 0;
    }

    /* initialise new values */
    for (idx_t i = cached_len; i < new_len; i++) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            cache[i] = next_term(cache[i - 1]);
        }
    }
    cached_len = new_len;
    return cache[n];
}
