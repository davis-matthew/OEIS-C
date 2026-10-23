/*
 * A166323  Numbers in which all the digits are larger than the arithmetic
 *          mean of their two neighbors.
 * https://oeis.org/A166323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base, easy
 * Formula:   a(n) = the nth decimal integer (with at least 3 digits) such
 *            that for every internal digit d_i (2 ≤ i ≤ L-1), 2*d_i > d_{i-1}
 *            + d_{i+1}
 * Range:     u8 n <= 83, u16 n <= 2328, u32 n <= 3569, u64 n <= 3569,
 *            u128 n <= 3569, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the sequence by length in ascending order.
   For each length L ≥ 3 it builds the numbers lexicographically by a
   depth‑first search, using the necessary constraint.  When the nth number
   is found it is returned. */

#include <stdint.h>
#include <stdbool.h>

static bool dfs(int pos, int len, unsigned char *digits, idx_t *cnt, idx_t target, seq_t *out) {
    if (pos == len) {
        seq_t val = 0;
        for (int i = 0; i < len; ++i)
            val = val * 10 + digits[i];
        (*cnt)++;
        if (*cnt == target) {
            *out = val;
            return true;
        }
        return false;
    }

    if (pos == 0) {                 /* first digit, non‑zero */
        for (int d = 1; d <= 9; ++d) {
            digits[0] = d;
            if (dfs(1, len, digits, cnt, target, out))
                return true;
        }
    } else if (pos == 1) {           /* second digit, any 0‑9 */
        for (int d = 0; d <= 9; ++d) {
            digits[1] = d;
            if (dfs(2, len, digits, cnt, target, out))
                return true;
        }
    } else {                        /* pos ≥ 2, must satisfy constraint */
        int prev2 = digits[pos - 2];
        int prev1 = digits[pos - 1];
        int bound = 2 * prev1 - prev2;   /* d < bound   (strict) */
        if (bound <= 0)
            return false;
        int maxd = bound - 1;
        if (maxd > 9) maxd = 9;
        for (int d = 0; d <= maxd; ++d) {
            digits[pos] = d;
            if (dfs(pos + 1, len, digits, cnt, target, out))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    /* The OEIS offset is 1 → a(1) is the first term. */
    if (n == 0) return (seq_t)0;  /* undefined; guard though n≥1 */

    idx_t cnt = 0;
    seq_t result = 0;
    unsigned char digits[30];     /* sufficient for all terms in the limit */

    for (int len = 3; ; ++len) {
        if (dfs(0, len, digits, &cnt, n, &result))
            return result;
    }
}
