/*
 * A121476  Palindromes with digit sum 9.
 * https://oeis.org/A121476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth palindrome whose decimal digits sum to 9.
   The sequence is the OEIS A121476, indexed from 1. */

#include <stdint.h>

static inline seq_t build_pal(const int *digits, idx_t k, int middle)
{
    seq_t res = (seq_t)0;
    /* left half + middle */
    for (idx_t i = 0; i < k; ++i)
        res = res * (seq_t)10 + (seq_t)digits[i];
    res = res * (seq_t)10 + (seq_t)middle;
    /* right half (reverse of left) */
    for (idx_t i = 0; i < k; ++i)
        res = res * (seq_t)10 + (seq_t)digits[k - 1 - i];
    return res;
}

static void dfs_gen(idx_t pos, idx_t k, int *digits, int sum, idx_t *cnt,
                    idx_t target, seq_t *result)
{
    if (pos == k) {
        int middle = 9 - 2 * sum;          /* 1 ≤ middle ≤ 9 because sum ∈ [1,4] */
        ++(*cnt);
        if (*cnt == target) {
            *result = build_pal(digits, k, middle);
        }
        return;
    }

    int start = (pos == 0) ? 1 : 0;
    int rem   = 4 - sum;                  /* remaining sum allowed */
    for (int d = start; d <= 9 && d <= rem; ++d) {
        digits[pos] = d;
        dfs_gen(pos + 1, k, digits, sum + d, cnt, target, result);
        if (*cnt >= target) return;       /* early exit */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, value undefined */

    if (n == 1)                          /* the only 1‑digit palindrome */
        return (seq_t)9;

    /* Find the odd length k such that C(k+4,4) ≥ n.
       C(k+4,4) = (k+4)(k+3)(k+2)(k+1)/24  provides the count of terms
       up to length 2k+1. */
    idx_t k = 1;
    idx_t cum = (k + 4) * (k + 3) * (k + 2) * (k + 1) / 24;
    while (cum < n) {
        ++k;
        cum = (k + 4) * (k + 3) * (k + 2) * (k + 1) / 24;
    }

    /* Number of terms before the current block (length 2k+1). */
    idx_t prev = (k + 3) * (k + 2) * (k + 1) * k / 24;
    idx_t offset = n - prev;              /* 1‑based within this block */

    int digits[64];                       /* half of the palindrome (k ≤ 60) */
    idx_t counter = 0;
    seq_t answer = (seq_t)0;

    dfs_gen(0, k, digits, 0, &counter, offset, &answer);
    return answer;
}
