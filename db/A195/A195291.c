/*
 * A195291  Years in the Gregorian calendar which are not (proleptic) leap
 *          years.
 * https://oeis.org/A195291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer that is *not* a Gregorian leap
 *            year. In each 400‑year cycle there are 303 non‑leap years. This
 *            function computes the nth non‑leap year by binary searching for
 *            the smallest year whose count of non‑leap years is at least n.
 *            The count of leap years up to year Y is fl...
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 0 */

    /* Rough upper bound: every 400 years there are 303 non‑leap years. */
    idx_t low  = 1;
    idx_t high = (n * 400ULL) / 303 + 400;   /* safety margin */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;

        /* count of non‑leap years ≤ mid */
        seq_t nonleap = (seq_t)mid
                        - ((seq_t)(mid / 4)
                           - (seq_t)(mid / 100)
                           + (seq_t)(mid / 400));

        if (nonleap >= n)
            high = mid;
        else
            low = mid + 1;
    }

    /* low is the nth non‑leap year */
    return (seq_t)low;
}
