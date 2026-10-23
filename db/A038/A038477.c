/*
 * A038477  Sums of 5 distinct powers of 5.
 * https://oeis.org/A038477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer whose base‑5 representation contains
 *            exactly five 1’s and the rest 0’s
 * Range:     u8 none, u16 n <= 21, u32 n <= 2002, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power5.h"

/* Computes binomial coefficient C(n,k).  Assumes 0 ≤ k ≤ 5 and n ≥ 0. */
static idx_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;      /* use symmetry */
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (unsigned __int128)res * (n - k + i) / i;
    }
    return res;
}

/* Return the n‑th term of A038477 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t idx = n - 1;            /* convert to 0‑based index */

    /* positions[i] = exponent of 5 at the i‑th least significant 1 bit
       (positions[0] < positions[1] < … < positions[4]) */
    idx_t positions[5];

    idx_t remaining = 5;          /* number of 1’s to place */
    for (int pos = 4; pos >= 0; --pos) {
        idx_t t = 0;
        /* find the largest t such that C(t, remaining) ≤ idx */
        while (binom(t + 1, remaining) <= idx) {
            ++t;
        }
        positions[pos] = t;
        idx -= binom(t, remaining);
        --remaining;
    }

    /* Sum 5^positions[i] for i = 0..4 */
    seq_t sum = 0;
    for (int i = 0; i < 5; ++i) {
        sum += power5(positions[i]);
    }
    return sum;
}
