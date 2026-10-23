/*
 * A041799  Denominators of continued fraction convergents to sqrt(420).
 * https://oeis.org/A041799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041799: Denominators of continued fraction convergents to sqrt(420)
   Recurrence: a(n) = 82*a(n-2) - a(n-4) for n > 3
   Initial values: a(0)=1, a(1)=2, a(2)=81, a(3)=164 */
 
#include <stdlib.h>
 
seq_t a(idx_t n)
{
    /* base case: n < 4 use the known initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)81;
    if (n == 3) return (seq_t)164;
 
    /* store the last four computed values: a[i] modulo 4 */
    seq_t a4[4];
    a4[0] = (seq_t)1;   /* a(0) */
    a4[1] = (seq_t)2;   /* a(1) */
    a4[2] = (seq_t)81;  /* a(2) */
    a4[3] = (seq_t)164; /* a(3) */
 
    /* compute iteratively up to n */
    for (idx_t i = 4; i <= n; ++i) {
        idx_t cur   = i & 3;         /* i % 4      */
        idx_t m2    = (i + 2) & 3;   /* (i-2) % 4  */
        idx_t m4    = i & 3;         /* (i-4) % 4  */
        seq_t val = (seq_t)82 * a4[m2] - a4[m4];
        a4[cur] = val;
    }
 
    return a4[n & 3]; /* a(n) is stored in slot n % 4 */
}
