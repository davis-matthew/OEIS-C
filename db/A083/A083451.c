/*
 * A083451  (n concatenated n times) - n^n.
 * https://oeis.org/A083451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (n repeated n times) - n^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 281 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"

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
    /* compute decimal digit count of n */
    idx_t temp = n;
    idx_t d = 1;
    while (temp >= 10) {
        temp /= 10;
        d++;
    }

    seq_t pow10_d = pow10(d);
    seq_t concat;

    /* use closed‑form if it does not overflow a 64‑bit build */
    if (((unsigned long long)d * (unsigned long long)n) <= 19ULL) {
        seq_t pow10_dn = pow10(d * n);
        seq_t numerator = pow10_dn - (seq_t)1;
        seq_t denominator = pow10_d - (seq_t)1;
        concat = ((seq_t)n * numerator) / denominator;
    } else {
        /* iterative concatenation for large n (both 64‑bit and big‑int) */
        seq_t cur = (seq_t)0;
        for (idx_t i = 0; i < n; ++i) {
            cur = cur * pow10_d + (seq_t)n;
        }
        concat = cur;
    }

    seq_t pownn = ipow((seq_t)n, (seq_t)n);
    return concat - pownn;
}
