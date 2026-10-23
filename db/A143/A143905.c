/*
 * A143905  Positive integers n that are palindromic in base 2 and whose
 *          binary representation has the same number of 0's as 1's.
 * https://oeis.org/A143905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 8788, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th smallest integer that is a binary palindrome and has
        equal numbers of 0's and 1's.  The sequence starts with
        9, 153, 165, ... (OEIS A143905).  The algorithm enumerates
        binary palindromes of length L = 4,8,12,…; for each L we count
        the palindromes that satisfy the bit‑balance condition, skip
        whole blocks until the desired index lies in the current block,
        and then construct that palindrome by combinatorial selection
        of the 1‑bits in the first half.  This runs in O(n) time and
        uses only 64‑bit arithmetic for all values needed for n ≤ 10000.  */

#include <stdint.h>
#include <stdbool.h>

/* ---------- small combinatorial helper ---------- */
static inline uint64_t binom_uint64(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = (res * (n - k + i)) / i;
    }
    return res;
}

/* ---------- construct the m‑th palindrome of length L ---------- */
static inline seq_t build_palindrome(uint64_t L, uint64_t m)
{
    uint64_t first_half_len = L / 2;
    uint64_t k = first_half_len - 1;          /* remaining positions after the leading 1 */
    uint64_t ones_total = L / 4 - 1;          /* ones to place in the remaining positions */

    seq_t val = ((seq_t)1 << (L - 1));        /* leading 1 at the most significant bit */

    uint64_t ones_needed = ones_total;
    uint64_t rem_bits = k;

    for (uint64_t pos = 1; pos <= k; ++pos) {
        /* determine if we should set this position to 1 or 0 */
        uint64_t count_zero = binom_uint64(rem_bits - 1, ones_needed);
        if (m < count_zero) {
            /* leave bit 0 */
        } else {
            /* set bit 1 */
            val |= ((seq_t)1 << (L - 1 - pos));
            m   -= count_zero;
            --ones_needed;
        }
        --rem_bits;
    }

    /* mirror the first half to obtain the full palindrome */
    for (uint64_t i = 0; i < first_half_len; ++i) {
        if ((val >> (L - 1 - i)) & 1)
            val |= ((seq_t)1 << i);
    }

    return val;
}

/* ---------- main sequence routine ---------- */
seq_t a(idx_t n)
{
    /* n is 1‑based */
    uint64_t target = (uint64_t)n;

    for (uint64_t L = 4; ; L += 4) {
        /* count of valid palindromes of this length */
        uint64_t cnt = binom_uint64(L / 2 - 1, L / 4 - 1);
        if (target > cnt) {
            target -= cnt;          /* skip this block */
        } else {
            /* the desired palindrome lies in this block */
            return build_palindrome(L, target - 1);   /* 0‑based index inside the block */
        }
    }
}
