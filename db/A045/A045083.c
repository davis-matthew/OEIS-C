/*
 * A045083  Numbers whose base-4 representation contains exactly four 0's and
 *          two 3's.
 * https://oeis.org/A045083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑4 representation contains exactly four 0's
 *            and two 3's. The function builds the n‑th such number by
 *            combinatorial ranking
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static unsigned long long binom_calc(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned long long res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (unsigned long long)(n - k + i) / (unsigned long long)i;
    }
    return res;
}

static unsigned long long count_completions(int zeros, int threes, int rem)
{
    if (zeros < 0 || threes < 0) return 0;
    int needed = zeros + threes;
    if (needed > rem) return 0;
    int r = rem - needed;                /* positions for 1 or 2 */
    unsigned long long ways = binom_calc(rem, zeros);
    ways *= binom_calc(rem - zeros, threes);
    ways *= (1ULL << r);
    return ways;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe */
    size_t tn = (size_t)n;

    /* Find the minimal length L such that the cumulative count >= n */
    int L = 6;
    unsigned long long cumulative = 0;
    while (1) {
        unsigned long long c1 = binom_calc(L - 1, 4);
        c1 *= (unsigned long long)(L - 5);
        c1 *= (1ULL << (L - 6));

        unsigned long long c2 = 2ULL * binom_calc(L - 1, 4);
        c2 *= binom_calc(L - 5, 2);
        c2 *= (1ULL << (L - 6));

        unsigned long long countL = c1 + c2;
        if (cumulative + countL >= tn) break;
        cumulative += countL;
        ++L;
    }

    unsigned long long idx_within = tn - cumulative - 1; /* 0‑based index within this length */

    int zeros  = 4;
    int threes = 2;
    int rem    = L;
    seq_t val = 0;

    for (int pos = 0; pos < L; ++pos) {
        int digits[4];
        int dcnt = 0;
        if (pos == 0) {
            digits[dcnt++] = 1;
            digits[dcnt++] = 2;
            digits[dcnt++] = 3;
        } else {
            digits[dcnt++] = 0;
            digits[dcnt++] = 1;
            digits[dcnt++] = 2;
            digits[dcnt++] = 3;
        }

        for (int i = 0; i < dcnt; ++i) {
            int d = digits[i];
            if (d == 0 && zeros == 0) continue;
            if (d == 3 && threes == 0) continue;

            int zeros2  = zeros  - (d == 0 ? 1 : 0);
            int threes2 = threes - (d == 3 ? 1 : 0);
            int rem2    = rem - 1;

            unsigned long long cnt = count_completions(zeros2, threes2, rem2);
            if (idx_within < cnt) {
                /* choose this digit */
                val = val * ((seq_t)4) + (seq_t)d;
                zeros = zeros2;
                threes = threes2;
                rem = rem2;
                break;
            } else {
                idx_within -= cnt;
            }
        }
    }
    return val;
}
