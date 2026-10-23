/*
 * A199168  Numbers whose digits are a permutation of (0,...,m) for some m.
 * https://oeis.org/A199168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini
 * Formula:   a(n) = the n-th smallest positive integer that is a permutation
 *            of the digits {0,1,...,m} for some m (leading zero not allowed,
 *            except for the single digit 0). The sequence is constructed by
 *            ordering all such permutations by numeric value, starting with m
 *            = 0, then m = 1, m = 2, and so on
 * Range:     u8 n <= 6, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The function a implements the above definition. */
seq_t a(idx_t n)
{
    /* special case for the first term (0) */
    if (n == 1)
        return (seq_t)0;

    /* pre‑compute factorials up to a safe upper bound (m ≤ 9 for n ≤ 10000) */
    static bool init = false;
    static idx_t fact_idx[12];
    static seq_t fact_seq[12];
    if (!init) {
        fact_idx[0] = 1;
        fact_seq[0] = (seq_t)1;
        for (int i = 1; i < 12; ++i) {
            fact_idx[i] = fact_idx[i - 1] * (idx_t)i;
            fact_seq[i] = fact_seq[i - 1] * (seq_t)i;
        }
        init = true;
    }

    /* find the block m such that the n-th term lies in the permutations
       of {0,1,...,m} with a non‑zero leading digit.  */
    idx_t cumulative = 1ULL;   /* count for m = 0 */
    int m = 0;
    while (true) {
        if (m > 0) {
            /* count of valid numbers for this m: m * m! */
            idx_t count = (idx_t)m * fact_idx[m];
            if (n <= cumulative + count) {
                /* n belongs to this block */
                break;
            }
            cumulative += count;
        }
        ++m;
    }

    /* offset of n within the valid permutations of the current m */
    idx_t offset = n - cumulative - 1;          /* 0‑based */
    idx_t rank_in_all = fact_idx[m] + offset;   /* 0‑based rank among all permutations */

    /* generate the permutation with that rank */
    seq_t result = 0;
    bool used[12] = { false };   /* digits 0..m are the only ones used */

    for (int pos = 0; pos <= m; ++pos) {
        int remaining = m + 1 - pos;
        idx_t fact = fact_idx[remaining - 1];
        idx_t idx = rank_in_all / fact;
        rank_in_all = rank_in_all % fact;

        /* find the idx‑th unused digit */
        int chosen = 0;
        while (used[chosen]) ++chosen;          /* skip used digits at start */

        int k = 0;
        for (int d = chosen; d <= m; ++d) {
            if (used[d]) continue;
            if (k == idx) {
                chosen = d;
                break;
            }
            ++k;
        }

        used[chosen] = true;
        result = result * (seq_t)10 + (seq_t)chosen;
    }

    return result;
}
