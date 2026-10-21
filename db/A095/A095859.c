/*
 * A095859  Triangle read by rows: T(n,k) = (n-k)^(k^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r-k)^(k^2) where r = ceil((sqrt(8n+1)-1)/2) and k = n -
 *            r(r-1)/2
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 31, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        idx_t sum = mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev;
    seq_t base = (seq_t)(r - k);
    seq_t exp  = (seq_t)(k * k);
    return big_pow(base, exp);
}
