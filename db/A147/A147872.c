/*
 * A147872  Number of consistent sets of 7 irreflexive binary order
 *          relationships over n objects.
 * https://oeis.org/A147872
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)*(n-3)*(n-2)*(n-1)*n* (n^9 + 3*n^8 - 26*n^7 -
 *            168*n^6 - 259*n^5 + 1743*n^4 + 15044*n^3 + 40722*n^2 - 111060*n
 *            - 817320) / 5040
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5) {
        /* Not defined for n < 5, return 0 as a safe default */
        return (seq_t)0;
    }

    /* Use 128‑bit intermediate arithmetic when available.
       This guarantees correct results for seq_t up to 64 bits. */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 nin = (unsigned __int128)n;

    unsigned __int128 n1 = nin - 1;
    unsigned __int128 n2 = nin - 2;
    unsigned __int128 n3 = nin - 3;
    unsigned __int128 n4 = nin - 4;

    /* Product of five consecutive numbers */
    unsigned __int128 term1 = n4 * n3 * n2 * n1 * nin;

    /* Powers of n up to n^9 */
    unsigned __int128 pow2 = nin * nin;
    unsigned __int128 pow3 = pow2 * nin;
    unsigned __int128 pow4 = pow3 * nin;
    unsigned __int128 pow5 = pow4 * nin;
    unsigned __int128 pow6 = pow5 * nin;
    unsigned __int128 pow7 = pow6 * nin;
    unsigned __int128 pow8 = pow7 * nin;
    unsigned __int128 pow9 = pow8 * nin;

    /* Polynomial in n */
    unsigned __int128 poly =
        pow9
        + 3 * pow8
        - 26 * pow7
        - 168 * pow6
        - 259 * pow5
        + 1743 * pow4
        + 15044 * pow3
        + 40722 * pow2
        - 111060 * nin
        - 817320;

    unsigned __int128 num = term1 * poly;
    seq_t result = (seq_t)(num / 5040);
    return result;
#else
    /* Fallback path: compute everything in seq_t.
       Works correctly when the intermediate values all fit into
       the native seq_t width.  This is sufficient for builds
       where seq_t is arbitrary precision, or where the tested
       n are small enough that overflow does not occur. */
    seq_t nin = (seq_t)n;

    seq_t n1 = nin - 1;
    seq_t n2 = nin - 2;
    seq_t n3 = nin - 3;
    seq_t n4 = nin - 4;

    seq_t term1 = n4 * n3 * n2 * n1 * nin;

    seq_t pow2 = nin * nin;
    seq_t pow3 = pow2 * nin;
    seq_t pow4 = pow3 * nin;
    seq_t pow5 = pow4 * nin;
    seq_t pow6 = pow5 * nin;
    seq_t pow7 = pow6 * nin;
    seq_t pow8 = pow7 * nin;
    seq_t pow9 = pow8 * nin;

    seq_t poly =
        pow9
        + 3 * pow8
        - 26 * pow7
        - 168 * pow6
        - 259 * pow5
        + 1743 * pow4
        + 15044 * pow3
        + 40722 * pow2
        - 111060 * nin
        - 817320;

    seq_t result = (term1 * poly) / 5040;
    return result;
#endif
}
