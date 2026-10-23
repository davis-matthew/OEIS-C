/*
 * A189017  Zero-one sequence based on tetrahedral numbers:
 *          a(A000292(k))=a(k); a(A145397(k))=1-a(k); a(1)=0.
 * https://oeis.org/A189017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 0;
   For n > 1 let t = max { k | k(k+1)(k+2)/6 <= n }.
   If n equals the t‑th tetrahedral number, a(n) = a(t);
   otherwise, n lies in the complement set, a(n) = 1 - a(n - t). */
#include <stdint.h>

static seq_t tetrahedral(idx_t k)
{
    __int128 t = k;
    t = t * (k + 1) * (k + 2) / 6;
    return (seq_t)t;
}

/* return largest k such that tetrahedral(k) <= n */
static idx_t max_tetrahedral_leq(idx_t n)
{
    idx_t high = 1;
    while (tetrahedral(high) <= (seq_t)n)
    {
        high <<= 1;
    }
    idx_t low = 0;
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if (tetrahedral(mid) <= (seq_t)n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    idx_t t = max_tetrahedral_leq(n);
    seq_t t_val = tetrahedral(t);

    if (t_val == (seq_t)n)
    {
        return a(t);
    }
    else
    {
        idx_t k = n - t;       /* index of n in the complement */
        return ((seq_t)1) - a(k);
    }
}
