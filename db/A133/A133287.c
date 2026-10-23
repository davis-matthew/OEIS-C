/*
 * A133287  Decimal numbers that when converted to hexadecimal produce a
 *          number that uses the same digits as the original number.
 * https://oeis.org/A133287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A133287: Decimal numbers whose decimal digits equal the multiset of digits of their hexadecimal representation (digits 0–9 only).  
 * This implementation computes terms by brute‑force search, caching the results so that any n ≥ 1 can be retrieved in constant time after its first appearance. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms = NULL;      /* array of found terms */
    static size_t term_count = 0;    /* number of terms stored */
    static size_t term_cap   = 0;    /* array capacity */
    static idx_t  last_checked = 0;  /* next integer to test */

    /* Return cached value if already available. */
    if (n <= term_count)
        return terms[n - 1];

    /* Grow the cache array when needed. */
    if (term_count == term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 16;
        seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!p) {
            /* Out of memory – return 0 for safety. */
            return (seq_t)0;
        }
        terms = p;
        term_cap = new_cap;
    }

    /* Search for additional terms until we have at least n of them. */
    while (term_count < n) {
        idx_t cand = last_checked++;
        /* Count decimal digits and obtain decimal length. */
        idx_t  dec = cand;
        unsigned char dec_cnt[10] = {0};
        size_t dec_len = 0;
        if (dec == 0) {
            dec_cnt[0] = 1;
            dec_len = 1;
        } else {
            while (dec > 0) {
                unsigned d = dec % 10;
                dec_cnt[d]++;
                dec /= 10;
                dec_len++;
            }
        }

        /* Count hexadecimal digits (0–9 only). */
        idx_t  hex = cand;
        unsigned char hex_cnt[10] = {0};
        size_t hex_len = 0;
        int   hex_ok = 1;
        if (hex == 0) {
            hex_cnt[0] = 1;
            hex_len = 1;
        } else {
            while (hex > 0) {
                unsigned d = hex % 16;
                if (d >= 10) { hex_ok = 0; break; }
                hex_cnt[d]++;
                hex /= 16;
                hex_len++;
            }
        }

        /* If hex representation contains invalid digits or lengths differ, skip. */
        if (!hex_ok || hex_len != dec_len) {
            continue;
        }

        /* Compare digit multiplicities. */
        int equal = 1;
        for (int i = 0; i < 10; ++i) {
            if (dec_cnt[i] != hex_cnt[i]) {
                equal = 0;
                break;
            }
        }
        if (!equal) continue;

        /* Store the term. */
        terms[term_count++] = (seq_t)cand;
    }

    return terms[n - 1];
}
