/*
 * A014368  a(n) = bc, where n = C(b,2)+C(c,1), b>c>=0.
 * https://oeis.org/A014368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   To compute a(n) we find the largest integer b such that
 *            C(b,2)=b*(b-1)/2 ≤ n. Then c = n – C(b,2). The value a(n) is the
 *            decimal concatenation of b followed by c (digits of c may be
 *            more than one). Indices start at 1 (a(1) corresponds to n=1)
 * Range:     u8 n <= 64, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* number of decimal digits of a 64‑bit integer, 0 has 1 digit */
static int digits_uint64(uint64_t x)
{
    int d = 1;
    while (x >= 10) {
        x /= 10;
        ++d;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* binary search for the largest b with b*(b-1)/2 ≤ n  */
    idx_t lo = 0;
    idx_t hi = n + 2;          /* hi is an exclusive upper bound large enough */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid - 1) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t b = lo;                         /* largest satisfying b */
    unsigned __int128 tri_b = (unsigned __int128)b * (b - 1) / 2;
    idx_t c = n - (idx_t)tri_b;           /* 0 ≤ c < b */

    /* compute 10^digits(c) */
    int d = digits_uint64(c);
    seq_t pow10 = 1;
    for (int i = 0; i < d; ++i)
        pow10 *= (seq_t)10;

    /* concatenate b and c */
    seq_t result = ((seq_t)b) * pow10 + (seq_t)c;
    return result;
}
