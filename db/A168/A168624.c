/*
 * A168624  a(n) = 1 - 10^n + 100^n.
 * https://oeis.org/A168624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 - 10^n + 100^n (equivalently 100^n - 10^n + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided arbitrary‑precision power utility. */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 10^(2n) using efficient exponentiation. */
    idx_t exp2 = n + n;            /* 2*n */
    seq_t pow_n  = seq_pow(10, n);      /* 10^n   */
    seq_t pow_2n = seq_pow(10, exp2);   /* 10^(2n) = 100^n */

    /* a(n) = 100^n - 10^n + 1  (avoid underflow for unsigned seq_t). */
    seq_t res = pow_2n - pow_n;
    res += 1;
    return res;
}
