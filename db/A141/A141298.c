/*
 * A141298  a(n) = number of distinct substrings in the binary representation
 *          of n that each occur multiple times.
 * https://oeis.org/A141298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A141298:
   a(n) = number of distinct substrings of the binary representation
   of n that occur at least twice (substrings may start with 0). */
#include <stdlib.h>
#include <string.h>

/* Helper struct to store a substring by its value and length. */
typedef struct {
    uint64_t val;   /* substring value (bits only) */
    uint8_t  len;   /* substring length */
} sub_t;

/* Comparator for sorting sub_t arrays. */
static int cmp_sub(const void *p1, const void *p2)
{
    const sub_t *a = (const sub_t *)p1;
    const sub_t *b = (const sub_t *)p2;
    if (a->val < b->val) return -1;
    if (a->val > b->val) return 1;
    if (a->len < b->len) return -1;
    if (a->len > b->len) return 1;
    return 0;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Determine binary length (MSB first). */
    uint8_t bits[64];
    int len = 0;
    uint64_t tmp = n;
    while (tmp) {
        bits[len++] = tmp & 1;
        tmp >>= 1;
    }
    /* Reverse to MSB-first order. */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = bits[i];
        bits[i]     = bits[len - 1 - i];
        bits[len-1-i] = t;
    }

    /* Maximum number of substrings: len*(len+1)/2. */
    int maxSubs = len * (len + 1) / 2;
    sub_t *subs = (sub_t *)malloc(maxSubs * sizeof(sub_t));
    if (!subs) return (seq_t)0;    /* allocation failure – return 0 */

    int kcnt = 0;
    /* Enumerate all substrings. */
    for (int i = 0; i < len; ++i) {
        uint64_t sub = 0;
        for (int j = i; j < len; ++j) {
            sub = (sub << 1) | bits[j];
            subs[kcnt].val = sub;
            subs[kcnt].len = (uint8_t)(j - i + 1);
            ++kcnt;
        }
    }

    /* Sort substrings first by value then by length. */
    qsort(subs, kcnt, sizeof(sub_t), cmp_sub);

    /* Count those that appear more than once. */
    int repeated = 0;
    for (int i = 0; i < kcnt; ) {
        int j = i + 1;
        while (j < kcnt && cmp_sub(&subs[i], &subs[j]) == 0) ++j;
        if (j - i > 1) ++repeated;
        i = j;
    }

    free(subs);
    return (seq_t)repeated;
}
