/*
 * A059984  Concatenation of Łukasiewicz words.
 * https://oeis.org/A059984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A059984:
   concatenation of Łukasiewicz words sorted by ascending reverse‑lexicographic order. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial_ull.h"

/* Word container – maximum length 20 is more than enough for n up to 10 000. */
typedef struct {
    uint8_t digits[20];
    int     len;
} word_t;

/* Generate all Łukasiewicz words of a given length. */
static void generate_words(int len, uint8_t *buf, int pos,
                           int curSum, int rem, word_t *out, int *cnt)
{
    if (pos == len) {
        if (curSum == len - 1) {
            memcpy(out[*cnt].digits, buf, (size_t)len);
            out[*cnt].len = len;
            (*cnt)++;
        }
        return;
    }
    int max_d = pos - curSum;
    if (max_d > rem) max_d = rem;
    for (int d = 0; d <= max_d; ++d) {
        buf[pos] = (uint8_t)d;
        generate_words(len, buf, pos + 1, curSum + d,
                       rem - d, out, cnt);
    }
}

/* Comparator: ascending order on reversed word (compare from last digit). */
static int cmp_revlex(const void *p1, const void *p2)
{
    const word_t *w1 = (const word_t *)p1;
    const word_t *w2 = (const word_t *)p2;
    for (int i = w1->len - 1; i >= 0; --i) {
        if (w1->digits[i] != w2->digits[i])
            return (int)w1->digits[i] - (int)w2->digits[i];
    }
    return 0;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    idx_t pos = 0U;

    for (int len = 1; ; ++len) {
        int k   = len - 1;
        uint64_t word_count =
            binomial_ull(2 * k, k) / (uint64_t)(k + 1);
        idx_t digits_this_len = (idx_t)len * word_count;

        if (pos + digits_this_len <= n) {
            pos += digits_this_len;
            continue;                /* skip entire block of this length */
        }

        /* Need this length to locate the nth digit */
        int max_words = (int)word_count;
        word_t *words = (word_t *)malloc((size_t)max_words * sizeof(word_t));
        if (!words) return (seq_t)0;   /* allocation failure – unlikely */

        int cnt = 0;
        uint8_t buf[20];
        generate_words(len, buf, 0, 0, k, words, &cnt);

        /* Sort in ascending reverse‑lexicographic order */
        qsort(words, (size_t)cnt, sizeof(word_t), cmp_revlex);

        /* Scan digits until we reach position n */
        for (int wi = 0; wi < cnt; ++wi) {
            for (int di = 0; di < len; ++di) {
                if (pos == n) {
                    seq_t result = (seq_t)words[wi].digits[di];
                    free(words);
                    return result;
                }
                ++pos;
            }
        }
        free(words);  /* should not be reached */
    }
}
