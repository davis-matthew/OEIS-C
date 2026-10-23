/*
 * A183806  1/16 the number of (n+1)X5 binary arrays with no 2X2 subblock
 *          being a reflection across the shared element pair of any
 *          horizontal or vertical neighbor.
 * https://oeis.org/A183806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the empirical recurrence a(n) =
 *            6*a(n-1)+81*a(n-2)-425*a(n-3)-1968*a(n-4)+10277*a(n-5)+16576*a(n-6)
 *            -92864*a(n-7)-64803*a(n-8)+320262*a(n-9)+224743*a(n-10)-99955*a(n-11)
 *            -959780*a(n-12)-1547921*a(n-13)+2262032*a(n-14)+2523060*a(n-15)
 *            -1692000*a(n-16)-1053000*a(n-17) Initial values (a...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const int coeff[17] = {
    6, 81, -425, -1968, 10277, 16576,
    -92864, -64803, 320262, 224743, -99955,
    -959780, -1547921, 2262032, 2523060, -1692000,
    -1053000
};

static const unsigned long long init_val[17] = {
    27ULL, 237ULL, 1953ULL, 16953ULL, 143191ULL,
    1232595ULL, 10494847ULL, 89998565ULL, 768481445ULL,
    6579905737ULL, 56244325315ULL, 481279798699ULL,
    4115586970547ULL, 35208336763197ULL, 301125201891953ULL,
    2575843502271345ULL, 22031666094964551ULL
};

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static seq_t *memo = NULL;    /* array indices start at 0, we will ignore memo[0] */
    static size_t memo_cap = 0;   /* capacity of memo (elements) */
    static idx_t memo_len = 0;    /* highest index already computed (>=17 after init) */

    /* Initial allocation */
    if (memo == NULL) {
        memo_cap = 18;                         /* indices 0..17 */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) abort();                    /* out of memory */
        memo[0] = 0;                           /* unused */
        for (int i = 0; i < 17; ++i) {
            memo[i + 1] = (seq_t)init_val[i];
        }
        memo_len = 17;
    }

    /* Ensure capacity for index n */
    if ((size_t)(n + 1) > memo_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 2 * memo_cap) new_cap = 2 * memo_cap;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Return cached value if available */
    if (n <= memo_len) return memo[n];

    /* Compute missing values using the recurrence */
    for (idx_t i = memo_len + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 17; ++k) {
            idx_t idx = i - 1 - (idx_t)k;   /* a(i-(k+1)) */
            seq_t term = memo[idx];
            int c = coeff[k];
            if (c >= 0) {
                pos = pos + term * (seq_t)c;
            } else {
                neg = neg + term * (seq_t)(-c);
            }
        }
        memo[i] = pos - neg;    /* positive result is guaranteed */
    }
    memo_len = n;

    return memo[n];
}
