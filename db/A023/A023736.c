/*
 * A023736  Numbers with exactly 3 3's in their base-5 expansion.
 * https://oeis.org/A023736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7839, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with exactly three 3's in their base‑5 expansion (OEIS A023736)
   Formula: a(n) = the n-th positive integer whose base‑5 representation contains
   exactly three digits equal to 3.  Index starts at 1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Large enough static cache for up to 100000 terms (more than needed for n≈10000). */
    static seq_t cache[100000];
    static idx_t max_n = 0;          /* Number of terms already stored. */
    static seq_t last = 0;           /* Largest integer that has already been examined. */

    /* Generate terms until we have at least n of them. */
    while (max_n < n)
    {
        seq_t v = last + 1;          /* Next integer to test. */

        /* Count the digits 3 in the base‑5 expansion of v. */
        seq_t t = v;
        idx_t count3 = 0;
        while (t > 0)
        {
            if ((t % 5) == 3)
                ++count3;
            t = t / 5;
        }

        /* If v has exactly three 3's, store it. */
        if (count3 == 3)
            cache[max_n++] = v;

        last = v;
    }

    return cache[n - 1];
}
