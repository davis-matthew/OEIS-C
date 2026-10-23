/*
 * A045015  Numbers whose base-4 representation contains no 0's and exactly
 *          two 1's.
 * https://oeis.org/A045015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑4 representation contains no 0's and exactly
 *            two 1's
 * Range:     u8 n <= 31, u16 n <= 2815, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Helper: compute binomial coefficient C(n,k) with seq_t arithmetic. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Helper: compute 2^exp as seq_t. */
static seq_t pow2_seq(idx_t exp)
{
    return (seq_t)1 << exp;
}

/* Count how many valid numbers have a total length of L digits in base‑4. */
static seq_t count_length(idx_t L)
{
    /* L must be at least 2 because we need two 1's. */
    if (L < (idx_t)2) return (seq_t)0;
    seq_t choose = binom_seq(L, (idx_t)2);          /* C(L,2)   */
    seq_t pow    = pow2_seq(L - (idx_t)2);          /* 2^(L-2)  */
    return choose * pow;                           /* total count for this length */
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* Undefined for offset 0, but safe. */

    /* Find the length L of the nth valid number. */
    idx_t  L = (idx_t)2;
    seq_t  cum = (seq_t)0;
    seq_t  target = (seq_t)n;

    while (cum + count_length(L) < target) {
        cum += count_length(L);
        ++L;
    }

    /* Offset within the block of numbers of length L, 0‑based. */
    seq_t off = target - cum - (seq_t)1;

    /* Build the digit string by unranking. */
    seq_t  num = (seq_t)0;
    int    need = 2;          /* how many 1's still to place */

    for (idx_t pos = (idx_t)0; pos < L; ++pos) {
        idx_t rem = L - pos - (idx_t)1;  /* positions remaining after this one */

        /* Try digits 1,2,3 in ascending order. */
        int d;
        for (d = 1; d <= 3; ++d) {
            int need2 = need;
            if (d == 1) {
                if (need2 == 0) continue;
                --need2;
            }

            if (need2 < 0 || (idx_t)need2 > rem)
                continue;

            seq_t cnt = binom_seq(rem, (idx_t)need2) * pow2_seq(rem - (idx_t)need2);

            if (off < cnt) {
                /* This digit is chosen. */
                num = num * (seq_t)4 + (seq_t)d;
                if (d == 1) --need;
                break;
            } else {
                off -= cnt;
            }
        }
    }
    return num;
}
