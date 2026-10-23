/*
 * A023763  Nialpdromes: digits in base 7 are in nonincreasing order.
 * https://oeis.org/A023763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th non‑negative integer whose representation in
 *            base 7 has digits in non‑increasing order. The function
 *            constructs the n‑th term by first determining the length of its
 *            base‑7 representation and then unranking the lexicographically
 *            smallest non‑increasing digit sequence of that length
 * Range:     u8 n <= 71, u16 n <= 870, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t binom_nk(int n, int k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (int i = 1; i <= k; ++i)
        res = res * (uint64_t)(n - k + i) / (uint64_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* the first term (n==1) is 0 */
    if (n == 1) return (seq_t)0;

    /* k is the 0‑based rank among numbers with length ≥1 */
    idx_t k = n - 2;

    /* Find the length L such that the cumulative count of numbers
       of smaller lengths ≤ k. */
    idx_t L = 1;
    uint64_t cumulative = 0;
    uint64_t cnt;

    for (;;) {
        /* count of non‑increasing base‑7 numbers of length L */
        int nn = L + 6;
        cnt = binom_nk(nn, 6) - 1;   /* C(L+6,6) - 1 */
        if (k < cumulative + cnt) break;
        cumulative += cnt;
        ++L;
    }

    /* rank r within numbers of length L (0‑based) */
    uint64_t r = k - cumulative;

    /* Unrank to generate the digit sequence */
    seq_t result = 0;
    int prev = 6;                     /* previous digit (max allowed) */

    for (int pos = 1; pos <= (int)L; ++pos) {
        int min_digit = (pos == 1) ? 1 : 0;
        int chosen = min_digit;
        int rem_len = L - pos;        /* positions still to fill */

        /* iterate possible digits in increasing order */
        for (int d = min_digit; d <= prev; ++d) {
            uint64_t count_d = binom_nk(rem_len + d, d);
            if (r < count_d) {
                chosen = d;
                break;
            }
            r -= count_d;
        }

        result = result * (seq_t)7 + (seq_t)chosen;
        prev = chosen;
    }

    return result;
}
