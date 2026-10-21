/*
 * A234926  Primes which become squares when the digits are rotated once to
 *          the left.
 * https://oeis.org/A234926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime that, when rotated once to the left,
 *            becomes a perfect square
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 12, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   digit_len, floor_sqrt, grow_seq, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* current candidate number to examine */
    static seq_t cur = (seq_t)61;   /* start at the first prime with leading 6 */

    /* keep generating terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* determine the number of digits of cur */
        int d     = digit_len(cur);
        seq_t pow10d_minus1 = pow10((idx_t)(d - 1));          /* 10^(d-1) */
        seq_t pow10d        = pow10d_minus1 * (seq_t)10;      /* 10^d      */
        seq_t first_digit   = cur / pow10d_minus1;            /* leading digit */

        /* If the leading digit is not 6, jump to the next block starting with 6 */
        if (first_digit != (seq_t)6) {
            cur = (seq_t)6 * pow10d;   /* start of the next 6‑block */
            continue;
        }

        /* cur starts with 6, test if it is a prime */
        if (is_prime_big(cur)) {
            seq_t remainder   = cur % pow10d_minus1;            /* remove first digit */
            seq_t rotated     = remainder * (seq_t)10 + first_digit;  /* rotate left */
            seq_t sq          = floor_sqrt(rotated);
            if (sq * sq == rotated) {                          /* perfect square? */
                grow_seq(&terms, &terms_cap, terms_len + 1);
                terms[terms_len++] = cur;
            }
        }

        /* move to the next integer */
        cur = cur + (seq_t)1;
    }

    return terms[n - 1];   /* a(1) is the first term */
}
