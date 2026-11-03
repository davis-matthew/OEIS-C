/*
 * A037278  Replace n with concatenation of its divisors.
 * https://oeis.org/A037278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = concat_{d|n} d
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 29, u64 n <= 119, u128 n <= 359,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 8852 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for 0, return 0 safely */
        return 0;

    seq_t res = 0;

    /* store the larger divisors (those > sqrt(n)) */
    idx_t *big = NULL;
    size_t cap = 0;
    size_t len = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d_low  = i;
            idx_t d_high = n / i;

            /* append the low divisor immediately */
            int l = num_digits((seq_t)d_low);
            res = res * pow10(l) + (seq_t)d_low;

            if (d_low != d_high) {
                /* grow array if needed */
                if (len == cap) {
                    size_t nc = cap == 0 ? 16 : cap * 2;
                    idx_t *tmp = (idx_t *)realloc(big, nc * sizeof(idx_t));
                    if (!tmp) { /* allocation failure – best effort */
                        free(big);
                        return res;
                    }
                    big = tmp;
                    cap = nc;
                }
                big[len++] = d_high;
            }
        }
    }

    /* append the stored larger divisors in ascending order */
    for (size_t k = len; k > 0; --k) {
        idx_t d = big[k - 1];
        int l = num_digits((seq_t)d);
        res = res * pow10(l) + (seq_t)d;
    }

    free(big);
    return res;
}
