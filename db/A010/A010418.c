/*
 * A010418  Squares mod 57.
 * https://oeis.org/A010418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th smallest residue r (0 ≤ r < 57) such that r is a
 *            quadratic residue modulo 57 (i.e. r ≡ k² (mod 57) for some
 *            integer k)
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all distinct squares modulo 57, sorted increasingly. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t, used in static array size */

seq_t a(idx_t n)
{
    /* Precompute the sorted list of residues once. */
    static bool init = false;
    static seq_t residues[57];
    static size_t count = 0;

    if (!init)
    {
        /* Mark which residues are squares modulo 57. 57 is < 2^16, so
           32‑bit arithmetic suffices for intermediate products. */
        bool seen[57] = {0};
        for (unsigned i = 0; i < 57; ++i)
        {
            unsigned r = (i * i) % 57;  // 0 <= r < 57
            seen[r] = true;
        }
        /* Collect residues in ascending order. */
        for (unsigned r = 0; r < 57; ++r)
        {
            if (seen[r])
            {
                residues[count++] = (seq_t)r;
            }
        }
        init = true;
    }

    if (n < 1 || (size_t)n > count)
    {
        /* Out of bounds: return 0 (undefined for this sequence). */
        return (seq_t)0;
    }

    return residues[n - 1];
}
