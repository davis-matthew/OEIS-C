/*
 * A087677  Consider recurrence b(0) = n/3, b(n) = b(0)*floor(b(n-1));
 *          sequence gives first integer reached, or -1 if no integer is ever
 *          reached.
 * https://oeis.org/A087677
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first integer reached by b(0) = n/3; b(k) =
 *            b(0)*floor(b(k-1)) return that integer, or -1 if none is
 *            reached. (OEIS offset 6)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    /* work with seq_t numbers for all intermediate values */
    seq_t N = (seq_t)n;

    /* initial floor(b(0)) */
    seq_t t = N / 3;

    /* if n is divisible by 3, b(0) is already an integer */
    if (N % 3 == 0)
        return t;

    /* iterate until we find an integer or give up after a safe bound */
    const idx_t MAX_ITER = 10000;
    for (idx_t i = 0; i < MAX_ITER; ++i)
    {
        /* check whether b(k)=N*t/3 is an integer:
           N*t divisible by 3  <=> (N%3)*(t%3) mod 3 == 0   */
        if (((N % 3) * (t % 3)) % 3 == 0)
        {
            /* compute the integer value.  Use 128‑bit arithmetic when
               seq_t is 64‑bit to avoid overflow; the cast will work
               for arbitrary‑precision seq_t as well. */
            unsigned __int128 prod = (unsigned __int128)N * (unsigned __int128)t;
            return (seq_t)(prod / 3);
        }

        /* update floor(b(k)) = floor((N/3) * t) */
        unsigned __int128 prod = (unsigned __int128)N * (unsigned __int128)t;
        t = (seq_t)(prod / 3);
    }

    /* if no integer was found within the bound, signal failure */
    return (seq_t)-1;
}
