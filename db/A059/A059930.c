/*
 * A059930  Numbers n such that n and n^2 combined use different digits.
 * https://oeis.org/A059930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: full, pass, 22 terms through n = 22 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A059930 – numbers n such that the decimal digits of n
   and of n² together contain no repeated digit.  
   The function maintains a small cache of the 22 existing terms and
   generates further terms by brute‑force search. */

#include <stdbool.h>
#include <stddef.h>

static bool good_n(seq_t n)
{
    uint16_t mask = 0;           /* bits 0..9 for digits 0..9 */
    seq_t x = n;

    /* digits of n */
    while (x > 0) {
        int d = (int)(x % 10);
        if (mask & (1u << d))   return false;   /* repeated digit */
        mask |= (1u << d);
        x = x / 10;
    }

    /* digits of n² */
    seq_t y = n * n;
    while (y > 0) {
        int d = (int)(y % 10);
        if (mask & (1u << d))   return false;
        mask |= (1u << d);
        y = y / 10;
    }
    return true;
}

static seq_t        terms[22] = {};  /* cache of found terms            */
static idx_t        term_cnt  = 0;   /* number of terms stored (≤22)   */
static seq_t        next_candidate = 1;  /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */

    /* generate missing terms up to the requested index */
    while (term_cnt < 22 && term_cnt < n) {
        /* search for the next qualifying integer */
        while (term_cnt < 22 && !good_n(next_candidate)) {
            ++next_candidate;
        }
        if (term_cnt < 22) {
            terms[term_cnt++] = next_candidate;
            ++next_candidate;
        }
    }

    if (n <= term_cnt) return terms[n - 1];
    /* n is beyond the available terms (there are only 22) */
    return 0;
}
