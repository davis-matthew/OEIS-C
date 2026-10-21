/*
 * A091281  Central term in powers of the Lo-Shu Magic Square as a matrix.
 * https://oeis.org/A091281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091281: central term in powers of the Lo‑Shu magic square
   a(2k+1) = 5 * 15^(2k)       (k ≥ 0)
   a(2k)   = (15^(2k) + 2*24^k) / 3   (k ≥ 0)
   (a(0)=1)
   Uses exponentiation‑by‑squaring; works for all seq_t and idx_t types. */
#include <stdlib.h>

/* fast exponentiation: base^exp  (exp ≥ 0) */
static inline seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Odd n: a(2k+1) = 5 * 15^(2k)  -> exponent = n-1 */
    if (n & 1U) {
        idx_t exp = n - 1;                     /* exp = 2k */
        seq_t power = pow_seq_t((seq_t)15, exp);
        return power * (seq_t)5;
    }
    /* Even n: a(2k) = (15^(2k) + 2*24^k) / 3  -> k = n/2 */
    idx_t k = n >> 1;
    seq_t p15 = pow_seq_t((seq_t)15, n);        /* 15^(2k) */
    seq_t p24 = pow_seq_t((seq_t)24, k);        /* 24^k   */
    seq_t sum = p15 + (p24 * (seq_t)2);
    return sum / (seq_t)3;
}
