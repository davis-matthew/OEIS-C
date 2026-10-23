/*
 * A044292  Numbers k such that the string 4,5 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that the base‑9 representation of k contains the
 *            substring “45” but the representation of k‑1 does not
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool has45(seq_t v)
{
    seq_t prev = (seq_t)-1;
    while (v != 0) {
        seq_t digit = v % (seq_t)9;
        if (digit == 4 && prev == 5)
            return true;
        prev = digit;
        v /= (seq_t)9;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Lazily build the list of terms up to the requested index. */
    static seq_t *terms = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static seq_t last_checked = 0;

    if (n == 0) {
        return (seq_t)0; /* undefined for n==0, but harmless */
    }

    while (len < n) {
        seq_t k = last_checked + (seq_t)1;
        bool condition = has45(k) && !has45(k - (seq_t)1);
        if (condition) {
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
                cap = new_cap;
            }
            terms[len++] = k;
        }
        last_checked = k;
    }

    return terms[n - 1];
}
