/*
 * A023761  Nialpdromes: digits in base 5 are in nonincreasing order.
 * https://oeis.org/A023761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth nonnegative integer whose base‑5 digits are in
 *            nonincreasing order (OEIS A023761)
 * Range:     u8 n <= 58, u16 n <= 576, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) for small n (fits in idx_t). */
static inline idx_t nCk(idx_t n, idx_t k)
{
    idx_t res = 1;
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;           /* offset = 1 for 0 */

    /* Find the length L of the n‑th term. */
    idx_t L = 1;
    for (;;) {
        idx_t cum = nCk(L + 5, 5) - L;     /* cumulative counts including 0 */
        if (cum >= n) break;
        ++L;
    }

    /* Compute offset within the numbers of length L (0‑based). */
    idx_t prev_cum = nCk(L + 4, 5) - (L - 1);   /* cumulative up to length L-1 */
    idx_t offset   = n - prev_cum - 1;

    seq_t val = 0;
    int max_digit = 4;          /* maximum allowed digit at current position */

    for (int pos = 0; pos < (int)L; ++pos) {
        idx_t rem = L - pos - 1;
        int start = (pos == 0) ? 1 : 0;          /* first digit cannot be 0 */

        for (int d = start; d <= max_digit; ++d) {
            idx_t cnt = nCk(rem + d, d);         /* sequences starting with digit d */

            if (offset < cnt) {
                val = val * (seq_t)5 + (seq_t)d;
                max_digit = d;
                break;
            } else {
                offset -= cnt;
            }
        }
    }

    return val;
}
