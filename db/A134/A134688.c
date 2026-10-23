/*
 * A134688  Successive digits of the terms of A000203, the sum-of-divisors
 *          function.
 * https://oeis.org/A134688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth digit of the concatenation of σ(1),σ(2),σ(3),…
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static idx_t *cum_digits = NULL;
static size_t cum_cap   = 0;
static size_t cum_len   = 0;

/* Ensure that cum_digits[cum_len-1] >= target */
static void ensure_digits(idx_t target) {
    while (cum_len == 0 || cum_digits[cum_len - 1] < target) {
        idx_t k = (idx_t)cum_len + 1;            /* next n value          */
        seq_t s = sigma_of(k);                  /* σ(k)                  */
        idx_t d = (idx_t)digit_len(s);          /* number of digits in σ(k) */

        idx_t new_cum = (cum_len == 0) ? d
                           : cum_digits[cum_len - 1] + d;

        if (cum_len == cum_cap) {               /* grow array if needed   */
            size_t new_cap = cum_cap ? cum_cap * 2 : 16;
            idx_t *tmp = (idx_t *)realloc(cum_digits,
                                          new_cap * sizeof(idx_t));
            cum_digits = tmp;
            cum_cap = new_cap;
        }
        cum_digits[cum_len++] = new_cum;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                         /* safeguard            */

    ensure_digits(n);                            /* have enough digits   */

    idx_t k_index = 0;
    while (k_index < cum_len && cum_digits[k_index] < n) ++k_index;

    idx_t k = k_index + 1;                       /* σ(k) that contains the nth digit */
    idx_t prev_cum = (k_index == 0) ? 0 : cum_digits[k_index - 1];
    idx_t offset   = n - prev_cum - 1;           /* zero‑based within σ(k) */

    seq_t s = sigma_of(k);                       /* recompute σ(k)        */
    idx_t len = (idx_t)digit_len(s);             /* digits of σ(k)         */

    idx_t exp = len - 1 - offset;                /* power for 10^exp       */
    seq_t pow10_e = pow10(exp);                  /* 10^exp                 */
    seq_t div = s / pow10_e;                     /* shift out higher digits */
    seq_t digit = div % (seq_t)10;               /* last remaining digit   */

    return digit;
}
