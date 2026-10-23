/*
 * A131500  Radii of orbits of planets in solar system, in units of radius of
 *          orbit of Mercury, multiplied by 4.
 * https://oeis.org/A131500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 4;
   For n >= 2: a(n) = a(n-1) + min( 3*2^(n-3), 96 ); */

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for n < 1 – harmless */
        return 0;

    seq_t sum = (seq_t)4;      /* a(1) */

    if (n == 1)
        return sum;

    seq_t diff = (seq_t)3;     /* a(2) - a(1) */

    for (idx_t k = 2; k <= n; ++k) {
        sum += diff;          /* obtain a(k) */

        if (k < n) {          /* prepare diff for next step */
            if (k == 2)         /* keep diff = 3 for a(3) */
                diff = (seq_t)3;
            else if (diff < (seq_t)96)
                diff <<= 1;    /* double the difference */
            else
                diff = (seq_t)96; /* cap at 96 */
        }
    }

    return sum;
}
