/*
 * A107470  1-limiting word of the morphism 1->111133779, 2->113379, 3->228,
 *          4->113779, 5->1379, 6->28, 7->446, 8->46, 9->5.
 * https://oeis.org/A107470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth digit of the infinite word obtained by iterating the
 *            morphism 1 → 111133779, 2 → 113379, 3 → 228, 4 → 113779, 5 →
 *            1379, 6 → 28, 7 → 446, 8 → 46, 9 → 5 starting from the single
 *            symbol 1. The function expands the word until it contains at
 *            least n+1 symbols and returns the digit at positi...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* mapping tables for digits 1..9 */
static const char * const maps[10] = {
    "",          /* 0 unused */
    "111133779", /* 1 */
    "113379",    /* 2 */
    "228",       /* 3 */
    "113779",    /* 4 */
    "1379",      /* 5 */
    "28",        /* 6 */
    "446",       /* 7 */
    "46",        /* 8 */
    "5"          /* 9 */
};

/* pre‑computed lengths of each image */
static const uint8_t lens[10] = {
    0,   /* 0 unused */
    9,   /* 1 */
    6,   /* 2 */
    3,   /* 3 */
    6,   /* 4 */
    4,   /* 5 */
    2,   /* 6 */
    3,   /* 7 */
    2,   /* 8 */
    1    /* 9 */
};

seq_t a(idx_t n)
{
    /* start with the single symbol '1' */
    size_t cur_len = 1;
    uint8_t *cur = (uint8_t *)malloc(cur_len);
    if (!cur)
        return (seq_t)0;          /* allocation failure – unlikely */
    cur[0] = 1;

    /* grow the word until it contains at least n+1 symbols */
    while (cur_len <= (size_t)n) {
        /* compute length of next iteration */
        size_t new_len = 0;
        for (size_t i = 0; i < cur_len; ++i)
            new_len += lens[cur[i]];

        uint8_t *next = (uint8_t *)malloc(new_len);
        if (!next) {
            free(cur);
            return (seq_t)0;      /* allocation failure */
        }

        size_t pos = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            uint8_t sym = cur[i];
            const char *exp = maps[sym];
            for (const char *p = exp; *p; ++p)
                next[pos++] = (uint8_t)(*p - '0');
        }

        free(cur);
        cur = next;
        cur_len = new_len;
    }

    seq_t result = (seq_t)cur[n];
    free(cur);
    return result;
}
