/*
 * A055770  Largest factorial number which divides n.
 * https://oeis.org/A055770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest factorial number which divides n
 * Range:     u8 n <= 719, u16 n <= 9526, u32 n <= 9526, u64 n <= 9526,
 *            u128 n <= 9526, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* fac = k!   (k starts at 1, fac = 1) */
    seq_t fac = (seq_t)1;
    idx_t k = 1;

    while (1)
    {
        /* next factorial would be fac * (k+1) */
        idx_t k_next = k + 1;

        /* guard against overflow and check that the next factorial
           does not exceed n                       */
        seq_t lim = (seq_t)n / (seq_t)k_next;   /* n/(k+1) */
        if (fac > lim)
            break;

        seq_t next = fac * (seq_t)k_next;

        /* if n is not divisible by the next factorial, stop */
        if ((idx_t)n % (idx_t)next != 0)
            break;

        /* else accept next factorial and continue */
        fac = next;
        k   = k_next;
    }

    return fac;   /* largest factorial dividing n */
}
