/*
 * A017953  Expansion of 1/((1-3x)*(1-6x)*(1-11x)).
 * https://oeis.org/A017953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 117*a(n-2) + 198*a(n-3); with a(0)=1,
 *            a(1)=20, a(2)=283
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)283;

    /* Initialise the three most recent values. */
    seq_t a0 = (seq_t)1;    /* a(n-3) */
    seq_t a1 = (seq_t)20;   /* a(n-2) */
    seq_t a2 = (seq_t)283;  /* a(n-1) */
    seq_t an = a2;          /* placeholder for a(n) */

    /* Iterate up to the required index. */
    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) = 20*a2 - 117*a1 + 198*a0.
           To avoid any possibility of underflow (when the
           intermediate sum is smaller than the subtraction),
           compute the positive part first. */
        an = (seq_t)( (seq_t)20 * a2 + (seq_t)198 * a0 - (seq_t)117 * a1 );

        /* Slide the window: a0 ← a1, a1 ← a2, a2 ← an. */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
