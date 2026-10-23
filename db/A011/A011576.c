/*
 * A011576  Stirling numbers of second kind S2(27,n).
 * https://oeis.org/A011576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling2(27, n) (S(27,n))
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 7, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

static seq_t cache[28];
static bool ready = false;

seq_t a(idx_t n)
{
    if (n > 27)  /* S(27,n) = 0 for n > 27 */
        return (seq_t)0;

    if (!ready)
    {
        /* Use a single row to build the Stirling triangle up to n = 27 */
        seq_t row[28] = {0};
        row[0] = (seq_t)1;  /* S(0,0) = 1 */

        for (idx_t i = 1; i <= 27; ++i)
        {
            /* Compute row[i] down to row[1] using recurrence:
               S(i,k) = k*S(i-1,k) + S(i-1,k-1) */
            for (idx_t k = i; k >= 1; --k)
            {
                /* Perform multiplication/addition in a wider type to avoid
                   overflow when seq_t is 64‑bit; truncation only matters
                   for values that don't fit in seq_t. */
                unsigned __int128 prod = (unsigned __int128)k * (unsigned __int128)row[k];
                prod += (unsigned __int128)row[k - 1];
                row[k] = (seq_t)prod;
            }
            row[0] = (seq_t)0;  /* S(i,0) = 0 for i > 0 */
        }

        /* After the loop row[k] holds S(27,k); copy into cache. */
        for (idx_t k = 1; k <= 27; ++k)
            cache[(size_t)k] = row[k];

        ready = true;
    }

    return cache[(size_t)n];
}
