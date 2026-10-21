/*
 * A206852  Numbers N such that N/2 is a square, N/3 is a cube, and N/5 is a
 *          fifth power.
 * https://oeis.org/A206852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^15 * 3^10 * 5^6 * n^30
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 6, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required routine prototypes */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t, idx_t) */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t, seq_t) */

/* Computes the n-th term of OEIS A206852.  The term is defined by
   a(n) = 30233088000000 * n^30, where the constant factor is */
/* 2^15 * 3^10 * 5^6 = 30233088000000.  */
seq_t a(idx_t n)
{
    /* Compute base = 2^15 * 3^10 * 5^6 once. */
    static seq_t base = 0;
    static int init_done = 0;
    if (!init_done) {
        base = seq_pow((seq_t)2, 15);
        base = seq_mul(base, seq_pow((seq_t)3, 10));
        base = seq_mul(base, seq_pow((seq_t)5, 6));
        init_done = 1;
    }

    /* Compute n^30 and multiply by the constant base. */
    seq_t n_pow = seq_pow((seq_t)n, 30);
    return seq_mul(base, n_pow);
}
