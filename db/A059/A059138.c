/*
 * A059138  A hierarchical sequence (S(W3{2,2}cc) - see A059126).
 * https://oeis.org/A059138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*(-5 + 3*2^(1+n) - 2*n)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n <= 59 the shifting operation stays within the width
   of seq_t; for larger n we compute terms via the linear
   recurrence a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3).  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0,1,2 */
    if (n == 0) return (seq_t)18;
    if (n == 1) return (seq_t)90;
    if (n == 2) return (seq_t)270;

    /* Use the closed form when safe (n <= 59) */
    if (n <= 59) {
        /* Compute 2^(1+n) safely as an unsigned shift. */
        seq_t pow2 = ((seq_t)1) << (n + 1);          /* 2^(1+n)            */
        seq_t term = ((seq_t)3) * pow2;             /* 3 * 2^(1+n)         */
        term -= ((seq_t)2) * (seq_t)n;              /* - 2*n               */
        term -= (seq_t)5;                           /* - 5                 */
        return (seq_t)18 * term;                    /* 18 * term           */
    }

    /* For larger n we fall back to the recurrence.
       The intermediate multiplications are performed
       in unsigned arithmetic so overflow is defined
       (result modulo 2^width of seq_t).  */
    seq_t a0 = (seq_t)18;   /* a(n-3)   */
    seq_t a1 = (seq_t)90;   /* a(n-2)   */
    seq_t a2 = (seq_t)270;  /* a(n-1)   */
    seq_t an  = a2;         /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)4 * a2 - (seq_t)5 * a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
