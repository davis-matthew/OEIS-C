/*
 * A214531  List of words over {1,8} with equal numbers of 1's and 8's.
 * https://oeis.org/A214531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = lexicographically n‑th word of length 2k with * exactly k
 *            digits '1' and k digits '8', interpreted as a decimal * integer.
 *            Words are listed first by increasing length (k=1,2,3, …), * then
 *            in lexicographic order within each length
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 224, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Small binomial coefficient for 0 ≤ k ≤ n ≤ 32,
 * fits easily in idx_t for all required values. */
static idx_t binom_u64(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find the length k such that the term lies in the block of words with
     * length 2k (k≥1).  C(2k,k) words have that length. */
    idx_t total = 0;
    idx_t k = 0;
    while (1) {
        ++k;
        idx_t count = binom_u64(2 * k, k);
        if (total + count >= n)
            break;
        total += count;
    }

    /* Rank of the desired word within its block (0‑based). */
    idx_t rank = n - total - 1;

    /* Generate the word digit by digit. */
    seq_t value = 0;
    idx_t ones_left = k;
    for (idx_t pos = 0; pos < 2 * k; ++pos) {
        if (ones_left == 0) {
            /* Remaining positions must be '8'. */
            value = value * 10 + (seq_t)8;
            continue;
        }
        idx_t rem = 2 * k - pos;
        idx_t count = binom_u64(rem - 1, ones_left - 1);
        if (rank < count) {
            value = value * 10 + (seq_t)1;
            --ones_left;
        } else {
            value = value * 10 + (seq_t)8;
            rank -= count;
        }
    }
    return value;
}
