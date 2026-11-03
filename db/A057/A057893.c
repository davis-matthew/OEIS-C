/*
 * A057893  Negabinary numbral multiplication table.
 * https://oeis.org/A057893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = index_of( negabinary_value(i) * negabinary_value(j) )
 *            where (i,j) is the pair at antidiagonal position n. For a pair
 *            with sum s = i+j, the pairs are ordered as
 *            (0,s),(1,s-1),…,(s,0). The function negabinary_value(k)
 *            interprets the binary representation of k as a base‑(-2) number.
 *            The inver...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 84 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Invert: given an integer x, return the index k such that
   negabinary_value(k) == x.  The result is always non‑negative. */
static inline idx_t index_of(long long x)
{
    idx_t index = 0;
    int pos = 0;
    while (x != 0) {
        long long r = x % -2;
        x /= -2;
        if (r < 0) {           /* adjust negative remainder */
            r += 2;
            x += 1;
        }
        if (r) index |= ((idx_t)1 << pos);
        pos++;
    }
    return index;
}

/* Compute value of index k in base -2 (may be negative). */
static inline long long negabinary_value(idx_t k)
{
    long long val = 0;
    long long pow = 1;          /* (-2)^0 */
    idx_t mask = 1;
    while (mask <= k) {
        if (k & mask) val += pow;
        pow *= -2;
        mask <<= 1;
    }
    return val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine the antidiagonal s = i + j for position n */
    double d = sqrt((double)8.0 * (double)n + 9.0);
    idx_t s = (idx_t)((d - 3.0) / 2.0);

    unsigned __int128 start128 = (unsigned __int128)s * (s + 1) / 2;
    /* Adjust s up or down until start <= n < start + s + 1 */
    while ((unsigned __int128)n < start128) {
        s--;
        start128 = (unsigned __int128)s * (s + 1) / 2;
    }
    while ((unsigned __int128)n >= start128 + (s + 1)) {
        s++;
        start128 = (unsigned __int128)s * (s + 1) / 2;
    }

    idx_t i = n - (idx_t)start128;      /* i-th offset on the antidiagonal */
    idx_t j = s - i;                    /* j-th coordinate */

    long long fi = negabinary_value(i);
    long long fj = negabinary_value(j);
    long long prod = fi * fj;

    idx_t result = index_of(prod);
    return (seq_t)result;
}
