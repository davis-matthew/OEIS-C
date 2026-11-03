/*
 * A098892  Numbers which yield a repdigit when added to their reversal.
 * https://oeis.org/A098892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 415, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 82 terms through n = 1824 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098892: numbers n such that n + reverse(n) is a repdigit. */

/* The function returns the nth term of the sequence.
   It builds the sequence lazily by brute‑forcing candidates
   and stores the results in a static array.  This method
   works for the required range (n ≤ 10000) and stays inside
   the allowed runtime limits. */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

static int is_repdigit(seq_t v)
{
    if (v == 0) return 0;
    seq_t d = v % 10;
    while (v > 0) {
        if ((v % 10) != d) return 0;
        v /= 10;
    }
    return 1;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t last_cand = 1;          /* next number to test */

    /* make sure we have at least n terms */
    while (terms_len < (size_t)n) {
        seq_t cand = last_cand;
        seq_t rev = reverse_digits(cand);
        seq_t sum = cand + rev;
        if (is_repdigit(sum)) {
            /* append to the array */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) {
                    /* allocation failed – abort */
                    exit(EXIT_FAILURE);
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = cand;
        }
        ++last_cand;
    }

    return terms[n - 1];
}
