/*
 * A008541  Numbers that do not contain the letter `f'.
 * https://oeis.org/A008541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   copy_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose English spelling does not contain the letter 'f'.
   This implementation generates the sequence by testing each integer until
   the requested index is reached.  The result is memoised so that any order
   of queries works efficiently.  Uses only allowed operators.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/copy_str.h"

/* English words for units (0‑19) and tens (20‑90). */
static const char *const UNITS[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};
static const char *const TENS[10] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy",
    "eighty","ninety"
};

/* Recursive routine that writes the English representation of n into buf. */
static void word_num(idx_t n, char *buf, size_t *pos)
{
    if (n < 20) {
        copy_str(UNITS[n], buf, pos);
        return;
    }
    if (n < 100) {
        int t = n / 10;
        copy_str(TENS[t], buf, pos);
        int r = n % 10;
        if (r) {
            buf[(*pos)++] = '-';
            copy_str(UNITS[r], buf, pos);
        }
        return;
    }
    if (n < 1000) {
        int h = n / 100;
        copy_str(UNITS[h], buf, pos);
        buf[(*pos)++] = ' ';
        copy_str("hundred", buf, pos);
        int r = n % 100;
        if (r) {
            buf[(*pos)++] = ' ';
            word_num(r, buf, pos);
        }
        return;
    }
    if (n < 1000000) {          /* thousands */
        int th = n / 1000;
        word_num(th, buf, pos);
        buf[(*pos)++] = ' ';
        copy_str("thousand", buf, pos);
        int r = n % 1000;
        if (r) {
            buf[(*pos)++] = ' ';
            word_num(r, buf, pos);
        }
        return;
    }
    if (n < 1000000000) {       /* millions */
        int mi = n / 1000000;
        word_num(mi, buf, pos);
        buf[(*pos)++] = ' ';
        copy_str("million", buf, pos);
        int r = n % 1000000;
        if (r) {
            buf[(*pos)++] = ' ';
            word_num(r, buf, pos);
        }
        return;
    }
    /* For larger numbers, extend as needed. */
    int bi = n / 1000000000;
    word_num(bi, buf, pos);
    buf[(*pos)++] = ' ';
    copy_str("billion", buf, pos);
    int r = n % 1000000000;
    if (r) {
        buf[(*pos)++] = ' ';
        word_num(r, buf, pos);
    }
}

/* Returns true if the English name of n contains the letter 'f'. */
static bool has_f(idx_t n)
{
    if (n == 0) return false;   /* "zero" has no 'f' */
    char buf[128];
    size_t pos = 0;
    word_num(n, buf, &pos);
    for (size_t i = 0; i < pos; ++i)
        if (buf[i] == 'f')
            return true;
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_num = 0;

    if (n == 0) return 0;           /* undefined for offset < 1 */

    /* Allocate initial cache if necessary. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        if (!has_f(next_num)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;   /* double the capacity */
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_num;
        }
        ++next_num;
    }
    return cache[n - 1];
}
