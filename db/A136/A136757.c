/*
 * A136757  Leading digit of n! in base 6.
 * https://oeis.org/A136757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = leading decimal digit of n! when written in base 6
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static long double *pref = NULL;     /* pref[i] = log₆(i!)  */
static idx_t prefSize = 0;           /* next index that must exist in pref */

/* pre‑computed constant ln(6) to avoid repeated calls to logl() */
static const long double ln6 = logl(6.0L);

/*--------------------------------------------------------------------
 *  Compute the leading digit of n! in base 6.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* 0! = 1 → leading digit 1 */
    if (n == 0) return (seq_t)1;

    /* Expand the prefix table if needed */
    if (prefSize <= n) {
        idx_t newSize = n + 1;
        long double *newPref = (long double *)realloc(pref, newSize * sizeof(long double));
        if (!newPref) return (seq_t)0;      /* allocation failure – unlikely */

        pref = newPref;

        /* initialise the very first value */
        if (prefSize == 0) {
            pref[0] = 0.0L;
            prefSize = 1;
        }

        /* fill the table from prefSize up to n */
        for (idx_t i = prefSize; i <= n; ++i) {
            pref[i] = pref[i - 1] + logl((long double) i) / ln6;
        }
        prefSize = newSize;
    }

    /* Extract the fractional part of log₆(n!) */
    long double val  = pref[n];
    long double frac = val - floorl(val);

    /* Convert fractional part back to 6^frac  */
    long double pow6 = expl(frac * ln6);

    /* 1 ≤ pow6 < 6, round down to the leading digit */
    int leading = (int)(pow6 + 1e-14L);   /* tiny epsilon protects against
                                            floating‑point rounding */

    /* Guard against pathological rounding */
    if (leading == 0)  leading = 1;
    if (leading > 5)   leading = 5;

    return (seq_t)leading;
}
