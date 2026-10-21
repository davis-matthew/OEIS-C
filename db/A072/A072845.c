/*
 * A072845  Let S(1) = {1, 3, 7, 9}, S(n) be obtained from S(n-1) via 1 -> {1,
 *          3, 7, 9}, 3 -> {3, 9, 1, 7}, 7 -> {7, 1, 9, 3}, 9 -> {9, 7, 3, 1},
 *          then this sequence is the concatenation of S(1), S(2), ...
 * https://oeis.org/A072845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) follows the substitution S(1)={1,3,7,9}, S(k)=σ(S(k-1))
 *            with σ(1)={1,3,7,9}, σ(3)={3,9,1,7}, σ(7)={7,1,9,3},
 *            σ(9)={9,7,3,1}. a(n) is the concatenation of all S(k) for k≥1.
 *            Each term is one of {1,3,7,9}
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int trans[4][4] = {
    {1,3,7,9},   // σ(1)
    {3,9,1,7},   // σ(3)
    {7,1,9,3},   // σ(7)
    {9,7,3,1}    // σ(9)
};

static const seq_t base_digits[4] = {1,3,7,9};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, return 0 */
    /* find minimal k with sum_{i=1}^k 4^i >= n */
    idx_t k = 1;
    idx_t sum = 4;          /* 4^1 */
    idx_t power = 4;
    while (sum < n) {
        k++;
        power *= 4;         /* 4^k */
        sum += power;       /* cumulative sum */
    }
    /* compute index within S(k): 0-based */
    idx_t before = sum - power; /* sum of 4^1..4^{k-1} */
    idx_t pos = (n - 1) - before; /* 0 <= pos < power */

    /* get k base-4 digits of pos, most significant first */
    idx_t d[64]; /* enough space because k <= 7 for n<=10000 */
    int idx = k - 1;
    idx_t tmp = pos;
    while (idx >= 0) {
        d[idx] = tmp % 4;
        tmp /= 4;
        idx--;
    }

    /* the first digit selects the starting value from base_digits */
    seq_t cur = base_digits[d[0]];
    int cur_idx = d[0]; /* index 0..3 corresponding to cur */

    /* iterate over remaining digits */
    for (int i = 1; i < k; i++) {
        cur = trans[cur_idx][d[i]];
        /* update cur_idx for next step */
        if (cur == 1) cur_idx = 0;
        else if (cur == 3) cur_idx = 1;
        else if (cur == 7) cur_idx = 2;
        else cur_idx = 3;
    }
    return cur;
}
