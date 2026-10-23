/*
 * A044976  Numbers whose base-3 representation contains exactly two 0's and
 *          one 1.
 * https://oeis.org/A044976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Sequence of numbers whose base‑3 representation contains exactly
 *            two zeros and one one (OEIS A044976)
 * Range:     u8 n <= 25, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: compute binomial coefficient nCk using seq_t arithmetic.
   n and k are small (≤ ~30) and fit in idx_t. */
static seq_t binom_seq_t(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 to avoid contamination. */
    if (n == 0) return 0;

    /* --- Find the base‑3 length L of the n‑th number --- */
    idx_t L = 3;
    seq_t cumulative = 0;
    for (;; ++L)
    {
        seq_t Ls = (seq_t)L;
        seq_t count_len = (Ls - 1) * (Ls - 2) * (Ls - 2) / 2; /* (L-1)(L-2)²/2 */
        if (cumulative + count_len >= (seq_t)n)
            break;
        cumulative += count_len;
    }

    /* Rank of the target within its length group (0‑based). */
    seq_t k = (seq_t)n - cumulative - 1;

    /* Remaining counts of each digit for a length‑L number. */
    seq_t cnt0 = 2;          /* zeros remaining    */
    seq_t cnt1 = 1;          /* ones  remaining    */
    seq_t cnt2 = (seq_t)L - 3; /* twos  remaining    */
    seq_t value = 0;

    /* Construct the number digit by digit. */
    for (idx_t pos = 0; pos < L; ++pos)
    {
        idx_t rem = L - pos - 1;
        for (uint8_t d = 0; d <= 2; ++d)
        {
            if (pos == 0 && d == 0)          /* leading zero not allowed */
                continue;
            if (d == 0 && cnt0 == 0)
                continue;
            if (d == 1 && cnt1 == 0)
                continue;
            if (d == 2 && cnt2 == 0)
                continue;

            /* Counts after placing digit d. */
            seq_t r0 = cnt0;
            seq_t r1 = cnt1;
            seq_t r2 = cnt2;
            if (d == 0) r0--;
            else if (d == 1) r1--;
            else r2--;

            /* Number of permutations of the remaining positions. */
            seq_t perm_cnt = binom_seq_t(rem, (idx_t)r0);
            perm_cnt = perm_cnt * binom_seq_t(rem - (idx_t)r0, (idx_t)r1);

            if (k < perm_cnt)
            {
                value = value * (seq_t)3 + d;
                if (d == 0) cnt0--;
                else if (d == 1) cnt1--;
                else cnt2--;
                break;
            }
            k -= perm_cnt;
        }
    }
    return value;
}
