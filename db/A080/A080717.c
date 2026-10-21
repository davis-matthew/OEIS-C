/*
 * A080717  Let S(0) = 1; obtain S(k) from S(k-1) by applying 1 -> 2, 2 -> 31,
 *          3 -> 332; sequence is S(0), S(1), S(2), ...
 * https://oeis.org/A080717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080717:  a(n) is the nth digit of the infinite word obtained by
   iterating the morphism 1 → 2,  2 → 31,  3 → 332, starting from the
   symbol 1.  The word is built successively and the digits are output in
   the order they appear, starting with n = 1. */

#include <stdlib.h>
#include <string.h>

/* sequence storage */
static unsigned char *seq_buf      = NULL;   /* concatenated digits of the sequence */
static idx_t          seq_len      = 0;      /* number of digits stored          */
static idx_t          seq_cap      = 0;      /* capacity of seq_buf              */

/* current word for the morphism */
static unsigned char *word_buf      = NULL;  /* current word S(k)                 */
static idx_t          word_len      = 0;     /* length of current word            */

/* enlarge the sequence buffer if needed */
static void ensure_seq_cap(idx_t needed)
{
    if (seq_cap < needed) {
        size_t newcap = (seq_cap ? seq_cap * 2 : 16);
        while (newcap < needed) newcap *= 2;
        seq_buf = (unsigned char *)realloc(seq_buf, newcap);
        seq_cap = newcap;
    }
}

seq_t a(idx_t n)
{
    /* first call – initialise the first word S(0)=1 */
    if (seq_len == 0) {
        word_len = 1;
        word_buf = (unsigned char *)malloc(1);
        word_buf[0] = (unsigned char)1;          /* S(0) */
    }

    /* generate successive words until we have at least n digits in seq_buf */
    while (seq_len < n) {
        /* maximum possible length of the next word is 3 * current length */
        idx_t new_len = word_len * 3;
        unsigned char *new_word = (unsigned char *)malloc(new_len);
        idx_t pos = 0;

        /* expand the current word according to the morphism  */
        for (idx_t i = 0; i < word_len; ++i) {
            unsigned char d = word_buf[i];
            if (d == (unsigned char)1) {
                new_word[pos++] = (unsigned char)2;          /* 1 → 2 */
            } else if (d == (unsigned char)2) {
                new_word[pos++] = (unsigned char)3;          /* 2 → 3 1 */
                new_word[pos++] = (unsigned char)1;
            } else { /* d == 3 */
                new_word[pos++] = (unsigned char)3;          /* 3 → 3 3 2 */
                new_word[pos++] = (unsigned char)3;
                new_word[pos++] = (unsigned char)2;
            }
        }
        new_len = pos;        /* actual length after expansion */

        /* append the new word to the global sequence buffer */
        ensure_seq_cap(seq_len + new_len);
        memcpy(seq_buf + seq_len, new_word, new_len);
        seq_len += new_len;

        /* advance to the next word */
        free(word_buf);
        word_buf = new_word;
        word_len = new_len;
    }

    /* return the requested digit (digits are 1,2,3) */
    return (seq_t)seq_buf[n - 1];
}
