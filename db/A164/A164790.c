/*
 * A164790  a(n) is the smallest nonnegative number whose American English
 *          name has the letter "e" in the n-th position.
 * https://oeis.org/A164790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = the smallest nonnegative integer whose American English
 *            name has the letter 'e' at the n‑th position (1‑based)
 * Range:     u8 n <= 22, u16 n <= 43, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- English word tables ----- */
static const char *ones[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *tens[10] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

static const char *scales[11] = {
    "thousand","million","billion","trillion","quadrillion",
    "quintillion","sextillion","septillion","octillion",
    "nonillion","decillion"
};

/* ----- helpers ----- */
static inline void append_str(char *buf, size_t *len, const char *s)
{
    while (*s) buf[(*len)++] = *s++;
}

/* Builds the American English name of n (0 <= n).  The result is
   written into *buf and the length is stored in *len.  The word
   contains no hyphens, spaces, or punctuation. */
static void word_append(uint64_t n, char *buf, size_t *len)
{
    if (n < 20) {
        append_str(buf, len, ones[n]);
        return;
    }
    if (n < 100) {
        size_t t = n / 10;
        append_str(buf, len, tens[t]);
        uint64_t r = n % 10;
        if (r) word_append(r, buf, len);
        return;
    }
    if (n < 1000) {
        uint64_t h = n / 100;
        word_append(h, buf, len);
        append_str(buf, len, "hundred");
        uint64_t r = n % 100;
        if (r) word_append(r, buf, len);
        return;
    }
    /* n >= 1000: find the largest power of 1000 that divides n */
    uint64_t pow = 1000;
    int idx = 0;                /* idx = 0 -> thousand, 1 -> million, ... */
    while (pow * 1000ULL <= n) {
        pow *= 1000ULL;
        idx++;
    }
    uint64_t high = n / pow;
    uint64_t low  = n % pow;

    word_append(high, buf, len);
    append_str(buf, len, scales[idx]);
    if (low) word_append(low, buf, len);
}

/* ----- main function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset starts at 1; n==0 never requested */
    idx_t num = 0;
    while (1) {
        char word[512];
        size_t len = 0;
        word_append(num, word, &len);
        if (len >= (size_t)n && word[n-1] == 'e') {
            return (seq_t)num;
        }
        ++num;
    }
}
