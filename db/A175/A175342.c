/*
 * A175342  Number of arithmetic progressions (where the difference between
 *          adjacent terms is either positive, 0, or negative) of positive
 *          integers that sum to n.
 * https://oeis.org/A175342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A049988(n) - A000005(n) where A049988(n) = number of
 *            arithmetic progressions of positive integers with non‑negative
 *            common difference that sum to n (difference 0 allowed), and
 *            A000005(n) = number of divisors of n. The implementation
 *            enumerates all lengths k (1 ≤ k ≤ n) and all possible n...
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 0 is outside the OEIS offset; return 0 for safety */
    if (n == 0) return 0;

    /* Count of progressions with non‑negative difference (A049988) */
    idx_t countA = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* constant progression (d == 0) */
        if (n % k == 0)
            ++countA;                 /* a = n/k, d = 0 */

        if (k < 2) continue;          /* no positive d for k == 1 */

        /* k >= 2  –  consider non‑zero differences (d >= 1) */
        idx_t t = k * (k - 1) / 2;     /* t = k*(k-1)/2 */
        if (t == 0) continue;         /* safety, occurs only for k==1 */

        /* N - t*d must remain positive → d < N / t */
        idx_t dmax = (n - 1) / t;     /* integer division, safe since n>=1 */
        for (idx_t d = 1; d <= dmax; ++d) {
            idx_t rem = n - t * d;    /* remaining part for k*a */
            if (rem % k == 0)         /* a = rem / k ≥ 1 automatically */
                ++countA;
        }
    }

    /* Number of divisors of n (A000005) */
    idx_t divcnt = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            divcnt *= (exp + 1);
        }
    }
    if (tmp > 1) divcnt *= 2;    /* remaining prime factor */

    /* Final formula: a(n) = 2*countA - divcnt */
    seq_t result = (seq_t)countA * 2 - (seq_t)divcnt;
    return result;
}
