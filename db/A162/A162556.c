/*
 * A162556  Numbers k which are concatenations k=x//y such that x^2 + y^2 -
 *          x*y = k.
 * https://oeis.org/A162556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers k which are concatenations k=x//y such that x^2 + y^2 -
 *            x*y = k
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for generated sequence */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap  = 0;
    static idx_t next_k = 1;            /* next integer to test */

    /* pre‑compute powers of ten up to 24 digits */
    static seq_t pow10[25];
    static bool pow_ok = false;
    if (!pow_ok) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 25; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;
        pow_ok = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_size < (size_t)n) {
        seq_t k = (seq_t)next_k;
        /* determine number of decimal digits of k */
        int len = 0;
        seq_t tmp = k;
        do {
            tmp /= (seq_t)10;
            ++len;
        } while (tmp != (seq_t)0);

        bool valid = false;
        for (int s = 1; s < len; ++s) {
            seq_t pow10s = pow10[s];
            seq_t y = k % pow10s;
            /* reject leading zeros in y */
            if (y < pow10s / (seq_t)10)
                continue;
            seq_t x = k / pow10s;

            /* compute x^2 + y^2 - x*y */
            seq_t lhs = x * x;
            seq_t yy  = y * y;
            lhs += yy;
            seq_t xy  = x * y;
            lhs -= xy;
            if (lhs == k) {
                valid = true;
                break;
            }
        }
        if (valid) {
            if (terms_size == terms_cap) {
                size_t newcap = terms_cap ? terms_cap * 2 : 64;
                seq_t *newptr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!newptr) abort();          /* allocation failure */
                terms = newptr;
                terms_cap = newcap;
            }
            terms[terms_size++] = k;
        }
        ++next_k;
    }
    return terms[n - 1];
}
