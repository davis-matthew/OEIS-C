/*
 * A204002  Symmetric matrix based on f(i,j)=min{2i+j,i+2j}, by antidiagonals.
 * https://oeis.org/A204002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = min{s + i, 2s – i} where s = the smallest integer with
 *            s*(s–1)/2 ≥ n, and i = n – (s–1)*(s–2)/2 (1 ≤ i ≤ s–1). This is
 *            the sequence obtained by reading the symmetric matrix M(i,j) =
 *            min{2i + j, i + 2j} along antidiagonals
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined offset – return 0 safely */
        return 0;

    /* Find s by binary search.  The interval [1, n+2] is sufficient
       because s ≤ n+1 for all n ≥ 1. */
    idx_t low = 1;
    idx_t high = n + 2;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)(mid - 1) * mid / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Number of terms before diagonal s */
    unsigned __int128 prev_tri = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t i = n - (idx_t)prev_tri;   /* 1 ≤ i ≤ s-1 */

    seq_t val1 = (seq_t)i + (seq_t)s;          /* i + s */
    seq_t val2 = (seq_t)2 * (seq_t)s - (seq_t)i; /* 2s – i */
    return val1 < val2 ? val1 : val2;
}
