/*
 * A045165  Numbers whose base-5 representation contains exactly two 0's and
 *          no 1's.
 * https://oeis.org/A045165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number whose base‑5 representation contains exactly
 *            two 0's and no 1's (OEIS A045165). The algorithm finds the
 *            appropriate base‑5 length, then builds the n‑th number
 *            combinatorially, avoiding any explicit enumeration beyond the
 *            required term. All arithmetic conforms to the restrictions...
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

/* Small helper: binomial coefficient (n choose k) for tiny n (n ≤ 50). */
static idx_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* Small helper: 3^e for tiny e. */
static idx_t _pow3(idx_t e)
{
    idx_t res = 1;
    while (e-- > 0) res *= 3;
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* 1. Determine the length L of the base‑5 number. */
    idx_t cumulative = 0;
    idx_t len = 3;
    idx_t count_len;
    while (1)
    {
        /* Count of eligible numbers of length len:
           choose 2 positions for zeros among positions 1..len-1,
           fill the remaining len-2 positions with 2,3,4 (3 choices each). */
        count_len = _binom(len - 1, 2) * _pow3(len - 2);
        if (cumulative + count_len >= n) break;
        cumulative += count_len;
        ++len;
    }

    /* 2. Position within the numbers of length len (0‑based). */
    idx_t k = n - cumulative - 1;            /* 0‑based offset */

    /* 3. Build the number digit by digit. */
    seq_t cur = 0;
    int zeros_left = 2;                     /* zeros still to be placed */
    const int first_digits[3] = {2, 3, 4};
    const int other_digits[4] = {0, 2, 3, 4};

    for (idx_t pos = 0; pos < len; ++pos)
    {
        const int *digits;
        int digits_count;
        if (pos == 0)
        {
            digits = first_digits;
            digits_count = 3;
        }
        else
        {
            digits = other_digits;
            digits_count = 4;
        }

        for (int di = 0; di < digits_count; ++di)
        {
            int d = digits[di];
            int tmp_zeros_left = zeros_left - (d == 0);
            if (tmp_zeros_left < 0) continue;

            idx_t rem = len - pos - 1;
            if (tmp_zeros_left > rem) continue;

            idx_t ways = _binom(rem, tmp_zeros_left) * _pow3(rem - tmp_zeros_left);
            if (k < ways)
            {
                /* Choose this digit and continue. */
                cur = cur * (seq_t)5 + (seq_t)d;
                zeros_left = tmp_zeros_left;
                break;
            }
            else
            {
                k -= ways;      /* skip all numbers starting with this digit */
            }
        }
    }
    return cur;
}
