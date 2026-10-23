/*
 * A045003  Numbers whose base-3 representation contains exactly two 0's and
 *          four 2's.
 * https://oeis.org/A045003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th smallest integer whose base‑3 representation
 *            contains exactly two 0's and four 2's
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return binomial coefficient C(n,k) as seq_t */
static inline seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;               /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (n + 1 - i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the base‑3 length L of the n-th number */
    idx_t L = (idx_t)6;
    seq_t cumulative = (seq_t)0;
    seq_t count = (seq_t)0;

    while (1) {
        /* Total arrangements of L digits with #0=2, #2=4, #1=L-6 */
        seq_t total = binom_seq(L, 2) * binom_seq(L - 2, 4);
        /* Arrangements that start with 0 (disallowed) */
        seq_t zero_first = ((seq_t)(L - 1)) * binom_seq(L - 2, 4);
        count = total - zero_first;
        if (n <= cumulative + count) break;
        cumulative += count;
        ++L;
    }

    seq_t kth = n - cumulative;  /* 1‑based rank within length L */

    /* Ranks are computed digit by digit (most significant first) */
    idx_t remaining = L;
    idx_t zeros = (idx_t)2;            /* remaining number of 0's */
    idx_t twos  = (idx_t)4;            /* remaining number of 2's */
    idx_t ones  = (idx_t)(L - 6);      /* remaining number of 1's */
    seq_t value = (seq_t)0;

    while (remaining > (idx_t)0) {
        for (int d = 0; d <= 2; ++d) {
            if (remaining == L && d == 0) continue;          /* leading digit cannot be 0 */
            idx_t z = zeros;
            idx_t o = ones;
            idx_t t = twos;
            if (d == 0) {
                if (z == (idx_t)0) continue;
                --z;
            } else if (d == 1) {
                if (o == (idx_t)0) continue;
                --o;
            } else { /* d == 2 */
                if (t == (idx_t)0) continue;
                --t;
            }

            idx_t rem = remaining - 1;
            seq_t ways = binom_seq(rem, z) * binom_seq(rem - z, t);

            if (kth <= ways) {
                value = value * (seq_t)3 + (seq_t)d;
                remaining = rem;
                zeros = z;
                ones  = o;
                twos  = t;
                break;
            } else {
                kth -= ways;
            }
        }
    }
    return value;
}
