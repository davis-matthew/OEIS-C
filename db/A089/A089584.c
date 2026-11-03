/*
 * A089584  Numbers n which are a proper multiple (>1) of A068505(n) (= n read
 *          in base m+1 where m = largest digit of n).
 * https://oeis.org/A089584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth integer >9, without the digit 9, for which
   if m is its largest decimal digit, the number obtained by interpreting
   it in base (m+1) divides it properly (n / y > 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: set of digits of x in reverse order, return length and maximum digit.
   Returns true if digit 9 was found. */
static bool get_digits_rev(seq_t x, unsigned char *dets, size_t *len, unsigned char *max_digit)
{
    size_t i = 0;
    unsigned char maxd = 0;
    bool has9 = false;
    while (x > 0) {
        unsigned char dig = (unsigned char)(x % 10);
        if (dig == 9) { has9 = true; }
        if (dig > maxd) maxd = dig;
        dets[i++] = dig;
        x /= 10;
    }
    *len = i;
    *max_digit = maxd;
    return has9;
}

/* Helper: interpret digits (most significant last) in given base. */
static seq_t interpret_in_base(const unsigned char *digits, size_t len, seq_t base)
{
    seq_t val = 0;
    for (size_t i = len; i-- > 0;) {
        val = val * base + digits[i];
    }
    return val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but avoid underflow */

    /* Static cache of computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;
    static seq_t candidate = 10;   /* start searching from 10 */

    /* Ensure cache growth. */
    if (terms_cnt < (size_t)n) {
        /* Grow cache if needed. */
        if (terms_cap == 0) {
            terms_cap = 32;
            terms = mem_new(seq_t, terms_cap);
        }
        /* Search until we have enough terms. */
        unsigned char digits[64];
        size_t digits_len;
        unsigned char max_digit;
        while (terms_cnt < (size_t)n) {
            /* Skip any number containing digit 9. */
            if (get_digits_rev(candidate, digits, &digits_len, &max_digit)) {
                candidate++;
                continue;
            }

            seq_t base = (seq_t)(max_digit + 1);
            seq_t y = interpret_in_base(digits, digits_len, base);
            if (y != 0 && candidate > y && candidate % y == 0) {
                /* Store term. */
                if (terms_cnt == terms_cap) {
                    terms_cap *= 2;
                    terms = mem_resize(seq_t, terms, terms_cap);
                }
                terms[terms_cnt++] = candidate;
            }
            candidate++;
        }
    }

    return terms[n - 1];
}
