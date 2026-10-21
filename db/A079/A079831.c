/*
 * A079831  Numbers k such that floor(reverse(k)/k) = 2.
 * https://oeis.org/A079831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   2*k <= reverse(k) < 3*k (k's decimal reverse lies between 2k and
 *            3k)
 * Range:     u8 n <= 8, u16 n <= 1352, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Reverse the digits of a positive integer of type seq_t. */
static seq_t reverse_seq(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

seq_t a(idx_t n) {
    /* Static cache of already found terms */
    static seq_t *terms = NULL;       /* array of found terms */
    static idx_t terms_size = 0;      /* allocated capacity */
    static idx_t terms_cnt = 0;       /* number of terms currently stored */
    static idx_t last_checked = 0;    /* last integer that has been tested */

    /* Return immediately if we already have the requested term */
    if (n <= terms_cnt && n > 0) {
        return terms[n - 1];
    }

    /* Expand cache as needed */
    if (terms == NULL) {
        terms_size = 128;
        terms = (seq_t *)malloc(terms_size * sizeof(seq_t));
    }

    /* Scan forward until we have at least n terms */
    while (terms_cnt < n) {
        last_checked++;                     /* next integer to test */
        seq_t k = (seq_t)last_checked;     /* current candidate */

        seq_t rev = reverse_seq(k);

        seq_t two_k = k * 2;
        seq_t three_k = k * 3;

        /* Property: 2*k <= reverse(k) < 3*k */
        if (rev >= two_k && rev < three_k) {
            /* Grow array if necessary */
            if (terms_cnt == terms_size) {
                terms_size *= 2;
                terms = (seq_t *)realloc(terms, terms_size * sizeof(seq_t));
            }
            terms[terms_cnt++] = k;
        }
    }

    return terms[n - 1];
}
