/*
 * A160754  Numbers k such that k, 2k, 3k and 4k use the same number of
 *          characters when expressed in Roman numerals.
 * https://oeis.org/A160754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th integer k (1‑based) such that the lengths of
 *            the Roman numerals * for k, 2k, 3k and 4k are all equal. The
 *            function a() computes this by searching * forward from the
 *            previous maximum k, storing found terms in a dynamic array
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Lengths of Roman numerals for digits 0..9 in units, tens and hundreds place */
static const int unit_len[10] = {0, 1, 2, 3, 2, 1, 2, 3, 4, 2};
static const int ten_len[10]  = {0, 1, 2, 3, 2, 1, 2, 3, 4, 2};
static const int hund_len[10] = {0, 1, 2, 3, 2, 1, 2, 3, 4, 2};

/* Compute the length of the Roman numeral representation of n.
 * The function works for all seq_t values; for n > 3999 the length is defined
 * as the number of 'M's contributed by the thousands digit plus the
 * standard representation of the remaining three digits.
 */
static seq_t roman_len(seq_t n)
{
    /* thousands digit (each 'M' counts as one character) */
    seq_t thousands = n / (seq_t)1000;
    seq_t len = thousands;                /* each 'M' contributes 1 character */

    /* last three digits */
    seq_t hundreds = (n / (seq_t)100) % (seq_t)10;
    seq_t tens     = (n / (seq_t)10)   % (seq_t)10;
    seq_t units    =  n                  % (seq_t)10;

    /* convert sub‑digit values to idx_t for array indexing */
    idx_t h = (idx_t)hundreds;
    idx_t t = (idx_t)tens;
    idx_t u = (idx_t)units;

    len += (seq_t)hund_len[h];
    len += (seq_t)ten_len[t];
    len += (seq_t)unit_len[u];
    return len;
}

/* Dynamic storage for sequence values */
static seq_t *terms = NULL;          /* array of found terms */
static size_t terms_cap = 0;          /* current capacity */
static size_t terms_cnt = 0;          /* number of terms stored */
static seq_t last_k = 0;             /* largest k examined so far */

/* Ensure that at least n terms are stored in 'terms' */
static void ensure_terms(idx_t n)
{
    if (terms == NULL) {
        terms_cap = 128;
        terms = (seq_t *)malloc(sizeof(seq_t) * terms_cap);
        if (!terms) exit(1);
    }

    while (terms_cnt < n) {
        /* Advance to the next integer candidate */
        last_k = last_k + (seq_t)1;
        seq_t k = last_k;

        seq_t l1 = roman_len(k);
        seq_t l2 = roman_len(k * (seq_t)2);
        seq_t l3 = roman_len(k * (seq_t)3);
        seq_t l4 = roman_len(k * (seq_t)4);

        /* If all four lengths are equal, record the term */
        if (l1 == l2 && l1 == l3 && l1 == l4) {
            if (terms_cnt == terms_cap) {
                size_t new_cap = terms_cap * 2 + 1;
                seq_t *tmp = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
                if (!tmp) exit(1);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt] = k;
            terms_cnt++;
        }
    }
}

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; return 0 as a safe value */
    ensure_terms(n);
    return terms[(size_t)(n - (idx_t)1)];
}
