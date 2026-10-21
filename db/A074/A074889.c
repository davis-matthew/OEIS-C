/*
 * A074889  Non-palindromic numbers such that the two largest proper divisors
 *          are palindromes having at least two digits and no other divisor is
 *          a palindrome with at least two digits.
 * https://oeis.org/A074889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th non‑palindromic integer whose two largest *
 *            proper divisors are palindromes with at least two digits and
 *            which * has no other proper divisor that is such a palindrome
 * Range:     u8 none, u16 n <= 46, u32 n <= 212, u64 n <= 212, u128 n <= 212,
 *            big any n
 * Helpers:   is_pal_ge2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_pal_ge2_uint64.h"

/* Test whether n satisfies the definition of A074889.  n is assumed
 * to fit in a 64‑bit unsigned integer. */
static bool candidate_ok(uint64_t n)
{
    /* n must not be a palindrome itself */
    if (is_pal_ge2_uint64(n)) return false;

    uint64_t largest1 = 0, largest2 = 0;   /* two largest proper divisors */
    unsigned palCnt   = 0;                 /* count of palindrome divisors ≥ 2 digits */

    uint64_t d;
    for (d = 2; d * d <= n; ++d) {
        if (n % d != 0) continue;
        uint64_t d1 = d;
        uint64_t d2 = n / d;

        /* process divisor d1 */
        if (d1 < n) {
            if (d1 > largest1) { largest2 = largest1; largest1 = d1; }
            else if (d1 > largest2) largest2 = d1;
            if (is_pal_ge2_uint64(d1)) {
                if (++palCnt > 2) return false;   /* more than two palindromes */
            }
        }

        /* process the paired divisor d2, skip if equal to d1 */
        if (d2 != d1 && d2 < n) {
            if (d2 > largest1) { largest2 = largest1; largest1 = d2; }
            else if (d2 > largest2) largest2 = d2;
            if (is_pal_ge2_uint64(d2)) {
                if (++palCnt > 2) return false;   /* more than two palindromes */
            }
        }
    }

    /* if the number is prime there are no proper divisors (except 1) */
    if (largest1 == 0) return false;

    /* we need exactly two palindrome divisors */
    if (palCnt != 2) return false;

    /* the two largest proper divisors must themselves be palindromes
       with at least two digits */
    if (!is_pal_ge2_uint64(largest1)) return false;
    if (!is_pal_ge2_uint64(largest2)) return false;

    return true;
}

/* Memoised list of found terms. */
static seq_t *terms      = NULL;
static size_t terms_cap  = 0;
static size_t terms_len  = 0;
static uint64_t last_checked = 1;          /* last integer examined */

/* Grow the terms array to hold at least n elements. */
static void ensure_n_terms(idx_t n)
{
    while (terms_len < n) {
        ++last_checked;
        if (candidate_ok(last_checked)) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 128;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);               /* memory allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_n_terms(n);
    return terms[n - 1];
}
