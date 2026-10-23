/*
 * A038228  Triangle whose (i,j)-th entry is binomial(i,j)*3^(i-j)*10^j.
 * https://oeis.org/A038228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j)*3^(i-j)*10^j where i = max{m : m*(m+1)/2
 *            <= n}, j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow10, pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

/* Find the row index i for a given global index n.
   It satisfies i*(i+1)/2 <= n < (i+1)*(i+2)/2.       */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 0xFFFFFFFFu;          /* enough for sqrt(2*UINT64_MAX) */
    idx_t best = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + (idx_t)1);
        val /= 2;
        if (val <= n) {
            best = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }
    return best;
}

seq_t a(idx_t n)
{
    idx_t i = find_row(n);
    unsigned __int128 base128 = (unsigned __int128)i * (i + (idx_t)1) / 2;
    idx_t base = (idx_t)base128;
    idx_t j = n - base;

    seq_t bin = binomial(i, j);
    seq_t p3 = pow3(i - j);
    seq_t p10 = pow10(j);

    seq_t res = seq_mul(bin, p3);
    res = seq_mul(res, p10);
    return res;
}
