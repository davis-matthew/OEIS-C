/*
 * A133109  Triangle read by rows, A042965 on the diagonal, 0 elsewhere.
 * https://oeis.org/A133109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = { A042965(r) if n = r(r+1)/2, else 0 }
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the r-th element of sequence A042965:
   A042965: 1,3,4,5,7,8,9,11,12,13,... (all positive integers except those congruent to 2 mod 4) */
static seq_t a042965_value(idx_t r)
{
    idx_t k = (r - 1) / 3;
    idx_t m = (r - 1) % 3;
    seq_t val = ((seq_t)4) * (seq_t)k + 1;
    if (m == 1)
        val += 2;
    else if (m == 2)
        val += 3;
    return val;
}

/* Compute the triangular row index r such that t_r = r(r+1)/2 <= n < t_{r+1} */
static idx_t triangular_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n) {
        hi <<= 1;
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo; /* t_lo <= n < t_hi */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t r = triangular_row(n);
    unsigned __int128 t = (unsigned __int128)r * (r + 1) / 2;
    if (t != n)
        return 0;               /* n is not a triangular number */
    return a042965_value(r);
}
