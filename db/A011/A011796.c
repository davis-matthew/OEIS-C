/*
 * A011796  Number of irreducible alternating Euler sums of depth 6 and weight
 *          6+2n.
 * https://oeis.org/A011796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( binom(n+5,5) - binom(n/2+2,2) * [n even] - (n/3+1) *
 *            [n≡0 mod3] + 1 * [n≡0 mod6] ) / 6 where [condition] is 1 if the
 *            condition holds, 0 otherwise Binomial coefficients are computed
 *            exactly using 128‑bit intermediates
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* As long as n ≥ 1 (offset 1).  Values for n = 0 are irrelevant. */
    if (n == 0) return 0;

    /* Compute binom(n+5,5) = (n+5)(n+4)(n+3)(n+2)(n+1) / 120 using 128‑bit ops */
    unsigned __int128 prod = 1;
    prod = prod * (unsigned __int128)(n + 5);
    prod = prod * (unsigned __int128)(n + 4);
    prod = prod * (unsigned __int128)(n + 3);
    prod = prod * (unsigned __int128)(n + 2);
    prod = prod * (unsigned __int128)(n + 1);
    seq_t binom5 = (seq_t)(prod / 120);

    /* Subtract binom(n/2+2,2) if n is even */
    seq_t sub2 = 0;
    if ((n & 1) == 0) {
        idx_t m = n / 2;
        unsigned __int128 prod2 = (unsigned __int128)(m + 2) * (unsigned __int128)(m + 1);
        sub2 = (seq_t)(prod2 / 2);
    }

    /* Subtract (n/3+1) if n is a multiple of 3 */
    seq_t sub3 = 0;
    if ((n % 3) == 0) {
        idx_t m = n / 3;
        sub3 = (seq_t)(m + 1);
    }

    /* Add 1 if n is a multiple of 6 */
    seq_t add6 = 0;
    if ((n % 6) == 0) add6 = 1;

    /* Combine everything and divide by 6 */
    seq_t val = binom5 - sub2 - sub3 + add6;
    val /= 6;

    return val;
}
