/*
 * A045160  Numbers whose base-5 representation contains no 0's and exactly
 *          three 1's.
 * https://oeis.org/A045160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A045160: numbers whose base‑5 representation contains no 0’s
   and exactly three 1’s, in increasing order. */

#include <stdbool.h>

/* small power of 3 – exp ≤ 64 */
static inline seq_t pow3_small(idx_t exp)
{
    seq_t res = 1;
    seq_t base = 3;
    while (exp > 0)
    {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* binomial coefficient C(n,k) – n,k fit in <64 */
static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* count of length‑L strings with digits 1…4, exactly three 1’s */
static inline seq_t count_len(idx_t L)
{
    if (L < 3) return 0;
    return binom_idx(L, 3) * pow3_small(L - 3);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range – return 0 */

    /* find length L so that the n-th number lies among L‑digit base‑5 strings */
    idx_t L = 3;
    seq_t cumulative = 0;
    while (1)
    {
        seq_t cnt = count_len(L);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++L;
    }

    seq_t r = n - cumulative - 1;  /* zero‑based rank within length L */
    seq_t result = 0;
    idx_t ones_left = 3;

    for (idx_t pos = 0; pos < L; ++pos)
    {
        idx_t rem = L - pos - 1;
        for (int d = 1; d <= 4; ++d)
        {
            int newOnesInt = ones_left - (d == 1);
            if (newOnesInt < 0) continue;
            if (newOnesInt > (int)rem) continue;

            idx_t newOnes = (idx_t)newOnesInt;
            seq_t cnt;
            if (rem < newOnes) cnt = 0;
            else cnt = binom_idx(rem, newOnes) * pow3_small(rem - newOnes);

            if (cnt <= r)
            {
                r -= cnt;
                continue;
            }

            result = result * (seq_t)5 + (seq_t)d;
            ones_left = newOnes;
            break;
        }
    }
    return result;
}
