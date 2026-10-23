/*
 * A110785  Palindromes in which the digits are in nonincreasing order halfway
 *          through.
 * https://oeis.org/A110785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 201, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   assemble_palindrome, binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A110785 – Palindromes whose left half is non‑increasing.
    For n ≥ 1 a(n) is the n‑th palindrome in lexicographic order
    that satisfies: the digits from the start up to and including the
    middle digit are non‑increasing.  The routine enumerates
    left‑half lengths k, counts how many palindromes of length
    2k‑1 (odd) and 2k (even) exist, then ranks the required
    left half using stars‑and‑bars combinatorics (binom):
        S_k = Σ_{d=1..9} C(d + k - 1, k - 1)
    where C refers to the binomial coefficient.  The selected
    left half is then mirrored to form the complete palindrome. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_small.h"
#include "../utils/assemble_palindrome.h"

/*  Count of nonincreasing sequences of length `len` whose maximum
    digit does not exceed `max_val`.  The count equals
        C(max_val + len, len)
    where C is the binomial coefficient.  */
static idx_t count_seq_remaining(idx_t len, uint8_t max_val)
{
    return (idx_t)binom_small(max_val + len, len);
}

/*  Build the left half of a palindrome of left‑half length `k`.
    `index` is 1‑based within all nonincreasing sequences of length `k`.
    The digits are written into `buf` (must have space for `k` digits). */
static void build_left_half(idx_t k, idx_t index, uint8_t *buf)
{
    idx_t remaining = k;              /* positions left to fill          */
    idx_t idx = index;                /* 1‑based index within this length */
    uint8_t max_digit = 9;             /* maximum allowed for next digit  */

    for (idx_t pos = 0; pos < k; ++pos)
    {
        int d_start = (pos == 0) ? 1 : 0;   /* leading digit ≥1           */
        for (int d = d_start; d <= max_digit; ++d)
        {
            idx_t cnt = count_seq_remaining(remaining - 1, d);
            if (idx <= cnt)
            {
                buf[pos] = (uint8_t)d;
                max_digit = d;
                --remaining;
                break;
            }
            idx -= cnt;
        }
    }
}

/*  Main routine: returns the nth term of A110785 (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined, but defined for safety */

    idx_t k = 1;                     /* left‑half length                 */
    while (1)
    {
        /* Count palindromes for this k (odd and even). */
        idx_t S_k = 0;
        for (int d = 1; d <= 9; ++d)
        {
            seq_t c = binom_small(d + k - 1, k - 1);
            S_k += (idx_t)c;
        }

        if (n <= S_k)                     /* odd length 2k-1 */
        {
            uint8_t left[20];              /* enough for all k used */
            build_left_half(k, n, left);
            return assemble_palindrome(left, k, true);
        }
        else if (n <= 2 * S_k)            /* even length 2k  */
        {
            uint8_t left[20];
            build_left_half(k, n - S_k, left);
            return assemble_palindrome(left, k, false);
        }

        n -= 2 * S_k;                     /* skip this length       */
        ++k;                               /* try next k             */
    }
}
