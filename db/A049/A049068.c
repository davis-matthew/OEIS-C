/*
 * A049068  Complement of quarter-squares (A002620).
 * https://oeis.org/A049068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n + A027434(n) – the nth positive integer that is not of
 *            the form floor(k^2/4). The function counts the numbers that are
 *            quarter‑squares (m^2 and m^2+m) up to a given bound and then
 *            binary–searches for the smallest m with enough
 *            non–quarter‑squares
 * Range:     u8 n <= 7, u16 n <= 1721, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Count of non‑quarter‑square numbers ≤ m (m ≥ 1).  A quarter‑square is
   of the form m^2 or m^2+m for some m ≥ 0. */
static inline seq_t count_nonqs(seq_t m)
{
    if (m == 0) return 0;
    /* Number of squares ≤ m: ⌊√m⌋. */
    seq_t s = floor_sqrt(m);
    /* Number of mixed terms m^2+m ≤ m.  Solve m^2+m ≤ m ⇒
       m ≤ ⌊(√(4m+1)-1)/2⌋.  Compute √(4m+1) and then floor the division. */
    seq_t t2sq   = (seq_t)4 * m + (seq_t)1;
    seq_t troot  = floor_sqrt(t2sq);
    seq_t t2     = (troot - (seq_t)1) / (seq_t)2;
    return m - (s + t2);          /* m minus the count of quarter‑squares */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* Guard: sequence starts at offset 1. */
    seq_t sn   = (seq_t)n;
    seq_t low  = (seq_t)1;
    seq_t high = sn;              /* start from n and grow if needed */
    while (count_nonqs(high) < sn) high <<= 1;  /* double until sufficient */

    /* Binary search for the smallest m with count_nonqs(m) ≥ n. */
    while (low < high) {
        seq_t mid = low + (high - low) / (seq_t)2;
        if (count_nonqs(mid) >= sn)
            high = mid;
        else
            low = mid + (seq_t)1;
    }
    return low;                    /* the nth term of the sequence */
}
