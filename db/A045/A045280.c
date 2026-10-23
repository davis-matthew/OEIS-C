/*
 * A045280  Numbers whose base-5 representation contains no 2's and exactly
 *          three 4's.
 * https://oeis.org/A045280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-5 representation contains no 2's and exactly
 *            three 4's
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* small binomial coefficient: n choose k, n,k fit in int */
static seq_t binomial_small(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* 3^exp, exp >= 0 */
static seq_t pow3_int(int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i) res *= 3;
    return res;
}

/* number of admissible numbers of base-5 length L */
static seq_t count_length(int L)
{
    if (L < 3) return 0;
    seq_t result = 0;

    /* leading digit is 4 */
    seq_t C1 = binomial_small(L - 1, 2);          /* choose the other two 4’s */
    seq_t term1 = C1 * pow3_int(L - 3);
    result += term1;

    /* leading digit is not 4 (it must be 1 or 3) */
    seq_t C2 = binomial_small(L - 1, 3);          /* choose all three 4’s among the rest */
    if (C2 > 0) {
        seq_t term2 = C2 * 2 * pow3_int(L - 4);   /* 2 choices for leading digit (1 or 3) */
        result += term2;
    }

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0, return 0 */

    seq_t target = (seq_t)n;
    seq_t cum = 0;
    int L = 3;

    /* find the length L that contains the n-th term */
    while (true) {
        seq_t cnt = count_length(L);
        if (cum + cnt >= target) break;
        cum += cnt;
        ++L;
    }

    seq_t k = target - cum - 1;  /* zero-based index within this length */

    int rem4 = 3;                /* remaining number of 4’s to place */
    int digits[128];             /* enough for any expected length */

    for (int pos = 0; pos < L; ++pos) {
        int allowed[4];
        int m = 0;
        if (pos == 0) {
            allowed[m++] = 1;
            allowed[m++] = 3;
            allowed[m++] = 4;
        } else {
            allowed[m++] = 0;
            allowed[m++] = 1;
            allowed[m++] = 3;
            allowed[m++] = 4;
        }

        for (int d = 0; d < m; ++d) {
            int digit = allowed[d];
            if (digit == 4 && rem4 == 0) continue;

            int rem4cand = rem4 - (digit == 4 ? 1 : 0);
            int M = L - pos - 1;

            seq_t ways = 0;
            if (rem4cand <= M && rem4cand >= 0) {
                seq_t bin = binomial_small(M, rem4cand);
                seq_t pow = pow3_int(M - rem4cand);
                ways = bin * pow;
            }

            if (ways <= k) {
                k -= ways;
                continue;
            } else {
                digits[pos] = digit;
                rem4 = rem4cand;
                break;
            }
        }
    }

    /* convert the collected digits to a number */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * 5 + digits[i];
    }
    return value;
}
