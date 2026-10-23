/*
 * A115924  Numbers k such that k and 4*k, taken together, are pandigital.
 * https://oeis.org/A115924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115924: Return the n‑th integer k (n≥1) such that the decimal digits of k
   and 4·k together contain each digit 0–9 exactly once.  The sequence is
   defined for all n≥1; values beyond the maximum possible term are
   returned as 0.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: the list of found terms, current candidate and sizes */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t current_k = 9999;   /* next candidate will be ++current_k */

    /* Generate more terms until we have at least n of them or we exhaust
       the admissible range (k < 25000).  */
    while (terms_len < n && current_k < 24999) {
        ++current_k;

        unsigned char used[10] = {0};
        unsigned int  count   = 0;
        bool bad = false;

        uint64_t tmp = current_k;                /* process digits of k   */
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 10);
            if (used[d]) { bad = true; break; }
            used[d] = 1;
            ++count;
            tmp /= 10;
        }
        if (bad) continue;

        tmp = (uint64_t)current_k * 4ULL;         /* process digits of 4*k */
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % 10);
            if (used[d]) { bad = true; break; }
            used[d] = 1;
            ++count;
            tmp /= 10;
        }
        if (bad) continue;

        if (count != 10) continue;               /* must use all 10 digits */

        /* Store the found term, expanding the array if needed. */
        if (terms_cap == 0) {
            terms_cap = 128;
            terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
            if (!terms) return (seq_t)0;           /* allocation failed */
        } else if (terms_len == terms_cap) {
            size_t new_cap = terms_cap * 2;
            seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_terms) return (seq_t)0;       /* allocation failed */
            terms = new_terms;
            terms_cap = new_cap;
        }
        terms[terms_len++] = (seq_t)current_k;
    }

    if (n == 0 || n > terms_len) return (seq_t)0;  /* n out of range */
    return terms[n - 1];
}
