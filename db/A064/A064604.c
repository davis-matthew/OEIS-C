/*
 * A064604  Partial sums of A001159: Sum_{j=1..n} sigma_4(j).
 * https://oeis.org/A064604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^4 * floor(n/k)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 115, u64 n <= 9767,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 9441 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Sum_{i=1..m} i^4  (m ≥ 0) */
static unsigned __int128 sum4_upto(idx_t m)
{
    if (m == 0) return (unsigned __int128)0;
    unsigned __int128 mm = m;
    unsigned __int128 a = mm;
    unsigned __int128 b = mm + 1;
    unsigned __int128 c = 2*mm + 1;
    unsigned __int128 d = 3*mm*mm + 3*mm - 1;
    unsigned __int128 prod = a * b;
    prod *= c;
    prod *= d;
    prod /= 30;                 /* exact integer division */
    return prod;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    unsigned __int128 total = 0;
    idx_t l = 1;
    while (l <= n) {
        idx_t q = n / l;             /* value of floor(n / k) on this block */
        idx_t r = n / q;             /* last k with the same q  */
        unsigned __int128 sum_range = sum4_upto(r) - sum4_upto(l - 1);
        total += sum_range * q;
        l = r + 1;                   /* move to next block */
    }

    return (seq_t)total;
}
