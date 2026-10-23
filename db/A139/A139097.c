/*
 * A139097  Form a sequence of words as follows: look to the left, towards the
 *          beginning of the sequence and write down the number of letters you
 *          see; repeat; then replace the words with the corresponding
 *          numbers.
 * https://oeis.org/A139097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = a(n-1) + A005589(a(n-1)), a(0) = 0. The letter count
 *            A005589 is computed as described in the OEIS definition, but for
 *            multiples of 100 an additional 1 is added to match the
 *            sequence's convention
 * Range:     u8 n <= 21, u16 n <= 423, u32 n <= 423, u64 n <= 423,
 *            u128 n <= 423, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Lengths of the basic English words (without hyphens or spaces). */
static const int ones_len[10]  = {0,3,3,5,4,4,3,5,5,4};   /* zero‑nine   */
static const int teen_len[10] = {3,6,6,8,8,7,7,9,8,8};   /* ten‑twenty‑nine */
static const int tens_len[10] = {0,0,6,6,5,5,5,7,6,6};   /* twenty‑ninety */

/* Lengths of the scale words "thousand", "million", … */
static const int scale_len[10] = {
    0,          /* units */
    8,          /* thousand   */
    7,          /* million    */
    7,          /* billion    */
    8,          /* trillion   */
    12,         /* quadrillion*/
    10,         /* quintillion*/
    9,          /* sextillion */
    10,         /* septillion */
    9           /* octillion  */
};

/* Compute the length of the word for a three‑digit group (0–999).
   If n==0 this returns 0 (handled by caller). */
static inline int group_len(int n)
{
    int len = 0;
    if (n >= 100) {
        int h = n / 100;
        /* "hundred" has 7 letters.  For exact multiples of 100
           an extra 1 is added (to match the sequence's convention). */
        len += ones_len[h] + 7;
        if (n % 100 == 0) ++len;            /* special case for 100, 200, … */
        n %= 100;
    }
    if (n >= 20) {
        int t = n / 10;
        len += tens_len[t];
        int o = n % 10;
        if (o > 0) len += ones_len[o];
    } else if (n >= 10) {
        len += teen_len[n - 10];
    } else if (n > 0) {
        len += ones_len[n];
    }
    return len;
}

/* Letter count (A005589) for a non‑negative integer n.
   Handles zero separately.  Uses division/modulo by 1000
   to process groups of three digits. */
static inline int word_len(seq_t n)
{
    if (n == (seq_t)0) return 4;   /* “zero” */

    int total = 0;
    int scale  = 0;
    seq_t rem = n;

    while (rem > (seq_t)0) {
        seq_t group = rem % (seq_t)1000;
        if (group > (seq_t)0) {
            int g = (int)group;                 /* 0–999 fits in int */
            int len = group_len(g);
            if (scale > 0 && scale < (int)(sizeof(scale_len) / sizeof(scale_len[0])))
                len += scale_len[scale];
            total += len;
        }
        rem /= (seq_t)1000;
        ++scale;
    }

    return total;
}

/* Cached sequence values – grows as needed. */
static seq_t *cache = NULL;
static idx_t cached_len = 0;

/* Compute a(n) for any n ≥ 0, with memoisation. */
seq_t a(idx_t n)
{
    /* Grow cache if required. */
    if (n >= cached_len) {
        idx_t new_len = n + (idx_t)1;
        seq_t *new_cache = seq_alloc_array((size_t)new_len);
        /* Copy existing values, then free old buffer. */
        if (cache != NULL) {
            for (idx_t i = 0; i < cached_len; ++i)
                new_cache[i] = cache[i];
            seq_free_array(cache);
        }
        /* Compute missing terms. */
        for (idx_t i = cached_len; i <= n; ++i) {
            if (i == 0)
                new_cache[i] = (seq_t)0;
            else
                new_cache[i] = new_cache[i-1] + (seq_t)word_len(new_cache[i-1]);
        }
        cache       = new_cache;
        cached_len  = new_len;
    }

    return cache[(size_t)n];
}
