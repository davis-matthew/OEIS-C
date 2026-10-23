/*
 * A175990  Irregular triangle read by rows: t(n,m) = binomial(n-m-1,m+1) for
 *          0 <= m <= floor((n-1)/2).
 * https://oeis.org/A175990
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy, less
 * Formula:   a(n) = binomial(n-m-1, m+1), where m is the * 0‑based position
 *            of n in the irregular triangle read by rows * starting with row
 *            n = 2. For n < 2 the return value is 0
 * Range:     u8 n <= 39, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Return the total number of terms up to and including row r
 * (rows start at r = 2). */
static inline idx_t terms_up_to(idx_t r)
{
    if (r < 2) return 0;
    idx_t m = r >> 1;          /* floor(r/2) */
    if (r & 1)   /* r odd: r = 2m+1 */
        return (m + 1) * (m + 1) - 1;
    else         /* r even: r = 2m   */
        return m * (m + 1) - 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2, behaviour below is arbitrary */

    idx_t w = n - 2;              /* zero‑based linear index */

    /* Find the smallest row r (r >= 2) such that terms_up_to(r) > w. */
    idx_t low = 2, high = 2;
    while (terms_up_to(high) <= w) high <<= 1;   /* ensure high is large enough */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (terms_up_to(mid) <= w)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    idx_t prev = terms_up_to(r - 1);
    idx_t pos = w - prev;                 /* 0‑based position in row r */

    /* Compute t(r, pos) = binomial(r - pos - 1, pos + 1) */
    return binomial(r - pos - 1, pos + 1);
}
