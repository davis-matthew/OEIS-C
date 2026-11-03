/*
 * A011266  a(n) = 2^(n*(n-1)/2)*n!.
 * https://oeis.org/A011266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(n-1)/2) * n! (equivalently a(n)=a(n-1)*n*2^(n-1),
 *            a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 77 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    seq_t res = (seq_t)1;
    seq_t two = (seq_t)1;  /* holds 2^(i-1) for the current i */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* multiply by i and 2^(i-1) */
        res = seq_mul(res, (seq_t)i);
        res = seq_mul(res, two);

        /* prepare 2^i for the next iteration */
        if (i < n) {
            two = seq_mul(two, (seq_t)2);
        }
    }
    return res;
}
