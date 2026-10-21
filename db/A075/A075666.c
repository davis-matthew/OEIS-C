/*
 * A075666  Sum of next n 5th powers.
 * https://oeis.org/A075666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^11 + 25*n^9 + 53*n^7 + 23*n^5 - 8*n^3) / 96
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 77, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (OEIS A075666) – Sum of the next n fifth powers.  */

#include <stdbool.h>

/* Compute a(n) with the closed‑form polynomial.  The function works
   when seq_t is any unsigned integer type sufficient to hold the
   result.  For 64‑bit or 128‑bit seq_t builds we use __int128
   intermediates to avoid overflow.  */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1, but guard against 0 */
        return 0;

#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit integers for intermediate calculations. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2   = nn * nn;          /* n²            */
    unsigned __int128 n3   = n2 * nn;          /* n³            */
    unsigned __int128 n5   = n3 * n2;          /* n⁵            */
    unsigned __int128 n7   = n5 * n2;          /* n⁷            */
    unsigned __int128 n9   = n7 * n2;          /* n⁹            */
    unsigned __int128 n11  = n9 * n2;          /* n¹¹           */

    unsigned __int128 num = 3 * n11
                          + 25 * n9
                          + 53 * n7
                          + 23 * n5
                          - 8 * n3;

    return (seq_t)(num / 96);
#else
    /* Fallback that uses seq_t only.  Assumes seq_t is large enough
       to contain all intermediate results for the values tested. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n5 = n3 * n2;
    seq_t n7 = n5 * n2;
    seq_t n9 = n7 * n2;
    seq_t n11 = n9 * n2;

    seq_t num = 3 * n11
              + 25 * n9
              + 53 * n7
              + 23 * n5
              - 8 * n3;

    return num / 96;
#endif
}
