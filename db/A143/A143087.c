/*
 * A143087  Triangle read by rows: T(n,k) = (2*2^min(k,n-k) -
 *          1)*binomial(n,k).
 * https://oeis.org/A143087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*2^min(k,r-k)-1) * binomial(r,k) where r =
 *            floor((sqrt(1+8*n)-1)/2), k = n - r(r+1)/2
 * Range:     u8 n <= 23, u16 n <= 83, u32 n <= 286, u64 n <= 1056,
 *            u128 n <= 1325, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

static inline idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t row = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 off = (unsigned __int128)mid * (mid + 1) / 2;
        if (off <= n)
        {
            row = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return row;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    unsigned __int128 off128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)off128;
    idx_t k = n - offset;

    idx_t exp = (k < (r - k)) ? k : (r - k);
    seq_t factor = pow2(exp + 1) - 1;

    seq_t bin = binom(r, k);

    return factor * bin;
}
