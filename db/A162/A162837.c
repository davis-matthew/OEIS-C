/*
 * A162837  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162837: Number of reduced words of length n in
   the Coxeter group on 33 generators with relations
   S_i^2 = (S_i S_j)^3 = I.  The values satisfy the
   linear recurrence
     a(0)=1, a(1)=33, a(2)=1056, a(3)=33264,
     a(n)=31*a(n-1)+31*a(n-2)-496*a(n-3)   for n≥4. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)33;
    if (n == (idx_t)2) return (seq_t)1056;
    if (n == (idx_t)3) return (seq_t)33264;

    seq_t prev3 = (seq_t)33;      /* a(1) */
    seq_t prev2 = (seq_t)1056;    /* a(2) */
    seq_t prev1 = (seq_t)33264;   /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = (seq_t)31 * prev1 + (seq_t)31 * prev2 - (seq_t)496 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
