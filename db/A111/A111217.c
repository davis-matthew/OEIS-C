/*
 * A111217  d_7(n), tau_7(n), number of ordered factorizations of n as n =
 *          rstuvwx (7-factorizations).
 * https://oeis.org/A111217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = ∏_{p^e || n} binomial(e+6,6) (OEIS A111217)
 * Range:     u8 n <= 23, u16 n <= 1439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square‑root: returns ⌊√n⌋ */
static inline idx_t integer_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n >> 1, mid, ans = 0;
    while (low <= high)
    {
        mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset guard; sequence is defined for n≥1 */

    seq_t result = 1;
    idx_t m = n;

    /* factor 2 separately */
    idx_t exp = 0;
    while ((m & 1ULL) == 0)
    {
        ++exp;
        m >>= 1;
    }
    if (exp > 0)
        result *= binomial(exp + 6, 6);

    /* factor odd prime powers */
    if (m > 1)
    {
        idx_t limit = integer_sqrt(m);
        for (idx_t p = 3; p <= limit; p += 2)
        {
            if (m % p != 0)
                continue;
            exp = 0;
            while (m % p == 0)
            {
                ++exp;
                m /= p;
            }
            result *= binomial(exp + 6, 6);
            limit = integer_sqrt(m);
        }
        /* remaining prime factor > 1, exponent 1 */
        if (m > 1)
            result *= binomial(1 + 6, 6);   /* binomial(7,6) = 7 */
    }

    return result;
}
