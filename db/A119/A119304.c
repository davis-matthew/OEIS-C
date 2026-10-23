/*
 * A119304  Triangle read by rows: T(n,k) = binomial(4n-k,n-k), 0 <= k <= n.
 * https://oeis.org/A119304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 65, u64 n <= 230, u128 n <= 860,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: a(n) = binomial(4r - c, r - c)
 * where r = floor((sqrt(8n+1)-1)/2) and c = n - r*(r+1)/2            */
#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);

    /* adjust r if the floating‑point estimate is off by 1 */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) r++;
    while ((unsigned __int128)r * (r + 1) / 2 > n) r--;

    /* compute column index c */
    idx_t prefix = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t c = n - prefix;

    /* compute the binomial coefficient */
    idx_t upper = 4 * r - c;   /* upper index */
    idx_t lower = r - c;       /* lower index */

    return binom(upper, lower);
}
