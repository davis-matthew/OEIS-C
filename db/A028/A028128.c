/*
 * A028128  Expansion of 1/((1-4x)(1-5x)(1-10x)(1-12x)).
 * https://oeis.org/A028128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028128 (Expansion of 1/((1-4x)(1-5x)(1-10x)(1-12x)))  
/* Recurrence: a(n)=22*a(n-1)-120*a(n-2)+5^(n+1)-4^(n+1), n>=2  
   Initial values: a(0)=1, a(1)=31 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)31;  /* a(1) */
    seq_t an;

    /* Start powers at 5^2 and 4^2, since we multiply once per loop to
       obtain the needed 5^(i+1) and 4^(i+1) for i >= 2. */
    seq_t pow5 = (seq_t)25;   /* 5^2 */
    seq_t pow4 = (seq_t)16;   /* 4^2 */

    for (idx_t i = 2; i <= n; ++i) {
        pow5 = pow5 * (seq_t)5;
        pow4 = pow4 * (seq_t)4;

        an = a_nm1 * (seq_t)22;          /* 22*a(i-1) */
        an = an - a_nm2 * (seq_t)120;    /* -120*a(i-2) */
        an = an + pow5;                  /* +5^(i+1) */
        an = an - pow4;                  /* -4^(i+1) */

        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return a_nm1;
}
