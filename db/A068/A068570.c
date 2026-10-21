/*
 * A068570  Numbers divisible by the number of letters to express them in
 *          English.
 * https://oeis.org/A068570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068570: numbers divisible by the number of letters in
   their English name.  The OEIS offset is 1, so a(1)=0.
   For n≥2, a(n) is the (n‑1)th positive integer k such that
   k % L(k) == 0, where L(k) is the length of the English spelling
   of k (without hyphens or spaces).  */
#include <stdlib.h>
#include <stdint.h>

static const unsigned char ones_len[10] = {0,3,3,5,4,4,3,5,5,4};   /* 1‑9 */
static const unsigned char teen_len[10] = {3,6,6,8,8,7,7,9,8,8};   /* 10‑19 */
static const unsigned char tens_len[10] = {0,0,6,6,5,5,5,7,6,6};    /* 20‑90 */
static const unsigned char scale_len[10] = {8,7,7,8,10,10,10,10,10,9}; /* thousand…decil... */

static seq_t letter_count(seq_t n)
{
    if (n == (seq_t)0)
        return (seq_t)4;   /* "zero" */
    unsigned int len = 0;
    seq_t tmp = n;
    unsigned int group_pos = 0;
    while (tmp > 0) {
        unsigned int group = (unsigned int)(tmp % 1000);
        tmp /= 1000u;
        if (group != 0) {
            unsigned int h = group / 100;
            unsigned int rest = group % 100;
            unsigned int t = rest / 10;
            unsigned int u = rest % 10;
            if (h > 0)
                len += ones_len[h] + 7;          /* "hundred" */
            if (t == 1) {
                len += teen_len[u];
            } else {
                if (t >= 2)
                    len += tens_len[t];
                if (u > 0)
                    len += ones_len[u];
            }
            if (group_pos > 0)
                len += scale_len[group_pos - 1];
        }
        ++group_pos;
    }
    return (seq_t)len;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t last_checked = 0;

    while (cache_size < (size_t)(n - 1)) {   /* need (n‑1) positive terms */
        ++last_checked;
        seq_t cand = last_checked;
        unsigned int lc = (unsigned int)letter_count(cand);
        if (lc != 0 && (cand % (seq_t)lc) == (seq_t)0) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;           /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
    }
    return cache[n - 2];   /* first cached term corresponds to a(2) */
}
