/*
 * A016633  Expansion of g.f. 1/((1-2*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A016633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 25,
   a(n) = 23*a(n-1) - 132*a(n-2) + 2^n   (n >= 2)  */
/* This computes any term of OEIS A016633, using only integer
   arithmetic.  The recurrence was chosen for its small number
   of operations and the fact that the power of two can be updated
   iteratively.  */
#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)25;
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)25;  /* a(1) */
    seq_t pow2 = (seq_t)2;    /* 2^1 */
    for (idx_t i = 2U; i <= n; ++i) {
        pow2 <<= 1;                        /* pow2 = 2^i */
        seq_t term = 23 * a_nm1;           /* 23*a(n-1) */
        term += pow2;                      /* + 2^n */
        term -= 132 * a_nm2;               /* - 132*a(n-2) */
        a_nm2 = a_nm1;                     /* shift window */
        a_nm1 = term;
    }
    return a_nm1;
}
