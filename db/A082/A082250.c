/*
 * A082250  a(n) = A082249(n)/7.
 * https://oeis.org/A082250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   compute_quotient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A082250:  a(n) = A082249(n)/7
 *  A082249(n) is the concatenation of 7 consecutive r‑digit
 *  decreasing numbers that is divisible by 7.  The generator
 *  scans increasing start values, computing the concatenation
 *  modulo 7; when it is divisible by 7 the exact quotient
 *  a(n) = (concatenation)/7 is produced and stored.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_quotient.h"

seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *terms = NULL;
    static idx_t terms_cap = 0;
    static idx_t terms_generated = 0;

    /* state for generating blocks */
    static int curr_digit = 2;          /* start with 2‑digit numbers */
    static idx_t next_start = 0;        /* next start value to test */

    /* ensure we have room for at least n terms */
    if (n > terms_cap) {
        idx_t new_cap = (n > 16) ? n : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (terms_generated < n) {
        /* compute the range of valid starts for the current digit length */
        idx_t min_start = 1;
        for (int i = 1; i < curr_digit; ++i)
            min_start *= 10;                  /* 10^(curr_digit-1) */
        idx_t max_start = min_start * 10 - 1;   /* 10^curr_digit - 1 */
        if (next_start < min_start + 6)
            next_start = min_start + 6;        /* ensure all nums have r digits */

        /* compute 10^curr_digit mod 7 for remainder calculation */
        uint8_t pow10_mod7 = 1;
        for (int i = 0; i < curr_digit; ++i)
            pow10_mod7 = (pow10_mod7 * 10) % 7;

        /* scan starts until we find one whose concatenation is divisible by 7 */
        while (next_start <= max_start) {
            idx_t s = next_start;
            int rem = 0;              /* concatenation modulo 7 */
            for (int k = 0; k < 7; ++k) {
                idx_t num = s - k;
                rem = (rem * pow10_mod7 + (uint8_t)(num % 7)) % 7;
            }
            ++next_start;             /* advance start for next try */
            if (rem != 0) continue;   /* not divisible by 7 */

            /* divisible – compute a(n) = concatenation / 7 */
            terms[terms_generated++] = compute_quotient(s, curr_digit);
            break;                    /* found this term, move on to next */
        }

        /* if we exhausted this digit length, move to the next one */
        if (next_start > max_start) {
            ++curr_digit;
            next_start = 0;           /* will be reset at loop start */
        }
    }

    return terms[n - 1];
}
