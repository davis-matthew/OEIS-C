/*
 * A210219  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210220; see the Formula section.
 * https://oeis.org/A210219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r*(r+1)/2 ≥ n and
 *            k = n - r*(r-1)/2. The triangle entry is T(r,k) = Σ_{j=0..⌊k/2⌋}
 *            binomial(k,2j) * binomial(r-j,k)
 * Range:     u8 n <= 31, u16 n <= 97, u32 n <= 337, u64 n <= 1248,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static idx_t find_row(idx_t pos)
{
    idx_t low = 1, high = 1;
    /* grow high until the triangular number exceeds pos */
    while (((unsigned __int128)high * (high + 1)) / 2 < pos) {
        if (high > (UINT64_MAX >> 1)) { /* prevent overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    /* binary search for minimal r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; n=0 is outside the sequence */
        return 0;

    idx_t r = find_row(n);
    idx_t prev = (r * (r - 1)) / 2;   /* elements before row r */
    idx_t k = n - prev;               /* 1‑based column index in row r */

    seq_t sum = 0;
    for (idx_t j = 0; j * 2 <= k; ++j) {
        seq_t c1 = binomial(k, 2 * j);
        seq_t c2 = binomial(r - j, k);
        sum += c1 * c2;
    }
    return sum;
}
