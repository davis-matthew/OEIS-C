/*
 * A060158  Number of permutations of [n] with 4 sequences.
 * https://oeis.org/A060158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - 7 + (6-n)*2^(n-1) - 3^n + 4^(n-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 184 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Computes the n-th term of OEIS sequence A060158.
 * a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 0
 * for n >= 5 uses the closed‑form formula. */
seq_t a(idx_t n)
{
    if (n <= 4) return 0;

    seq_t pow2_nm1 = pow2(n-1);   /* 2^(n-1) */
    seq_t pow3_n   = pow3(n);     /* 3^n     */
    seq_t pow4_nm1 = pow4(n-1);   /* 4^(n-1) */

    /* compute 2*n - 7 + 4^(n-1) first */
    seq_t result = ((seq_t)2*n) - 7;
    result += pow4_nm1;

    /* handle the (6-n)*2^(n-1) term in a way that never underflows:
     * for n==5 it is +1 * 2^(n-1),
     * for n==6 it is zero,
     * for n>=7 it is -(n-6) * 2^(n-1). */
    if (n == 5) {
        result += pow2_nm1;
    } else if (n > 6) {
        seq_t sub = (seq_t)(n-6) * pow2_nm1;
        result -= sub;
    }

    /* finally subtract 3^n */
    result -= pow3_n;
    return result;
}
