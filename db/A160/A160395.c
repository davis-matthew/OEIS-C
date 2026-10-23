/*
 * A160395  Each term is the previous term plus the number of letters in the
 *          previous number, as conventionally spelled out in British English.
 * https://oeis.org/A160395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(1)=1; a(n)=a(n-1)+letters_in_british_english(a(n-1)) for n>1
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* # of letters for 0..19 */
static const int ones_len[20] = {
    0, 3, 3, 5, 4, 4, 3, 5, 5, 4,
    3, 6, 6, 8, 8, 7, 7, 9, 8, 8
};

/* # of letters for tens 20..90 (index 2..9) */
static const int tens_len[10] = {
    0, 0, 6, 6, 5, 5, 5, 7, 6, 6
};

/* # of letters for scale words: thousand, million, billion, trillion, quadrillion */
static const int scale_len[6] = {0, 8, 7, 7, 8, 11};

/* length of number 0 <= n < 100 for British English */
static int word_len_0_to_99(seq_t n)
{
    if (n == (seq_t)0)
        return 0;
    if (n <= (seq_t)19) {
        return ones_len[(int)n];
    } else {
        seq_t tens = n / (seq_t)10;
        seq_t rem = n % (seq_t)10;
        int len = tens_len[(int)tens];
        if ((int)rem > 0)
            len += ones_len[(int)rem];
        return len;
    }
}

/* length of number 0 <= n < 1000 for British English */
static int word_len_triplet(seq_t n)
{
    if (n == (seq_t)0)
        return 0;
    int len = 0;
    seq_t h = n / (seq_t)100;
    if (h > (seq_t)0) {
        len += ones_len[(int)h] + 7;          /* "hundred" */
        seq_t rem = n % (seq_t)100;
        if (rem > (seq_t)0) {
            len += 3;                         /* "and" */
            len += word_len_0_to_99(rem);
        }
    } else {
        len += word_len_0_to_99(n);
    }
    return len;
}

/* length of arbitrary seq_t number for British English */
static int word_len_seq(seq_t num)
{
    if (num == (seq_t)0)
        return 0;
    int total = 0;
    int scale = 0;
    seq_t base1000 = (seq_t)1000;
    while (num > (seq_t)0) {
        seq_t trip = num % base1000;
        if (trip > (seq_t)0) {
            total += word_len_triplet(trip);
            if (scale > 0 && scale < (int)(sizeof(scale_len)/sizeof(scale_len[0])))
                total += scale_len[scale];
        }
        num = num / base1000;
        ++scale;
    }
    return total;
}

/* sequence a(n) with caching for arbitrary call order */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n < 1 */

    if (!cache) {
        size_t cap = (size_t)n + 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache[1] = (seq_t)1;
        cache_size = 1;
    }

    if (cache_size < n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i-1];
            int l = word_len_seq(prev);
            cache[i] = prev + (seq_t)l;
        }
        cache_size = n;
    }

    return cache[n];
}
