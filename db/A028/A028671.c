/*
 * A028671  Pseudo Galois numbers for d=9; order of group AGL(n,3^2).
 * https://oeis.org/A028671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n * Product_{k=0..n-1} (9^n - 9^k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Exponentiation by squaring: computes base^exp for seq_t base and idx_t exp. */
static seq_t seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (; exp; exp >>= 1) {
        if (exp & 1)
            result *= base;
        base *= base;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    seq_t base   = seq_pow_seq((seq_t)9, n); /* 9^n */
    seq_t prod   = base;
    seq_t pow9   = (seq_t)1;                /* 9^k, start with k=0 */

    for (idx_t k = 0; k < n; ++k) {
        seq_t term = base - pow9;         /* 9^n - 9^k */
        prod *= term;
        pow9 *= (seq_t)9;               /* next power of 9: 9^(k+1) */
    }

    return prod;
}
