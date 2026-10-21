/*
 * A052117  Number of nonnegative integer pairs (i,j) with binomial(i+r,r) +
 *          binomial(j+r,r) <= binomial(n+r,r), r=4.
 * https://oeis.org/A052117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(i,j) ≥ 0 | C(i+4,4) + C(j+4,4) ≤ C(n+4,4)}
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a binary search for each i, computing binomials on the fly. */
#include <stdbool.h>

static inline seq_t binom4(idx_t t)
{
    /* Computes C(t+4,4) = (t+4)(t+3)(t+2)(t+1)/24  */
    seq_t a = (seq_t)(t + 4);
    seq_t b = (seq_t)(t + 3);
    seq_t c = (seq_t)(t + 2);
    seq_t d = (seq_t)(t + 1);
    return ((a * b * c * d) / (seq_t)24);
}

seq_t a(idx_t n)
{
    seq_t fn = binom4(n);          /* upper bound C(n+4,4) */
    const seq_t base = binom4(0);  /* C(4,4) = 1, used in the early test */

    seq_t result = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t fi = binom4(i);

        /* If even the smallest j (0) fails, no j's are valid for this i. */
        if ((fi + base) > fn) {
            continue;
        }

        /* Binary search for the largest j with fi + binom4(j) <= fn. */
        idx_t lo = 0, hi = n;
        while (lo < hi) {
            idx_t mid = lo + ((hi - lo + 1) >> 1);
            seq_t fm = binom4(mid);
            if ((fi + fm) <= fn) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }

        /* lo is the largest admissible j; count all j = 0..lo (lo+1 values). */
        result += (seq_t)(lo + 1);
    }

    return result;
}
