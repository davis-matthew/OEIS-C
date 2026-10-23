/*
 * A189203  Zero-one sequence based on the sequence n(n+1)(2n+1)/6:
 *          a(A000330(k))=a(k); a(v(k))=1-a(k), where v=complement(A000330)
 *          and a(1)=0.
 * https://oeis.org/A189203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(A000330(k)) = a(k) if n = k(k+1)(2k+1)/6; a(v(k)) = 1 -
 *            a(k), where v is the complement of A000330; a(1)=0
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Compute the n-th tetrahedral (sum of squares) number */
static inline seq_t tetrahedral(idx_t k)
{
    unsigned __int128 a = k;
    unsigned __int128 b = k + 1;
    unsigned __int128 c = (unsigned __int128)2 * k + 1;
    unsigned __int128 val = a * b * c / 6;
    return (seq_t)val;
}

/* Upper bound for the search of k such that tetrahedral(k) <= n */
static inline idx_t max_k_est(idx_t n)
{
    /* tetrahedral(k) ~ k^3/3, so k <= (3n)^(1/3) + small margin */
    double d = pow((double)n * 3.0, 1.0 / 3.0);
    if (d < 1.0) d = 1.0;
    idx_t h = (idx_t)d + 3;
    if (h < 1) h = 1;
    return h;
}

/* Largest k such that tetrahedral(k) <= n */
static idx_t find_k_in_tetrahedral(idx_t n)
{
    idx_t low = 0;
    idx_t high = max_k_est(n);

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)tetrahedral(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0u;

    idx_t k_tetra = find_k_in_tetrahedral(n);
    seq_t val = tetrahedral(k_tetra);

    if (val == (seq_t)n)          /* n is a tetrahedral number */
        return a(k_tetra);

    /* n is in the complement; rank in the complement is n - k_tetra */
    return 1 - a(n - k_tetra);
}
