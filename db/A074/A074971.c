/*
 * A074971  Number of partitions of n into distinct parts of order n.
 * https://oeis.org/A074971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 719, u32 n <= 2519, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A074971 – Number of partitions of n into distinct parts whose lcm is n.
 *
 *  a(n) counts the subsets of the divisors of n that sum to n and have lcm n.
 *  The algorithm is dynamic programming over (current sum, current lcm), where
 *  the lcm values are restricted to the divisors of n.  This runs in
 *  roughly O(d(n)*n*d(n)), with d(n) the number of divisors of n; for n≤10000
 *  this is easily below the time limit.  The routine is fully C‑compatible
 *  and uses only the allowed integer operations.
 */

#include <stdint.h>
#include <stdlib.h>

/* --- auxiliary functions -------------------------------------------------- */

static idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- sequence function ---------------------------------------------------- */

seq_t a(idx_t n)
{
    /* a(0) is undefined for the offset 1 sequence, return 0 */
    if (n < 1) return 0;

    /* ---- 1. enumerate divisors of n ----------------------------------- */
    /* maximum number of divisors for n ≤ 10000 is far below 200 */
    uint64_t divs[128];
    idx_t dcnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            if (i != n / i) divs[dcnt++] = n / i;
        }
    }

    /* sort ascending – trivial bubble sort, small dcnt */
    for (idx_t i = 0; i < dcnt; ++i)
        for (idx_t j = i + 1; j < dcnt; ++j)
            if (divs[i] > divs[j]) {
                uint64_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    /* map divisor value → its index in divs[] for fast lookup of lcm state */
    idx_t maxDiv = n;
    int *idx_of = (int*)calloc((size_t)(maxDiv + 1), sizeof(int));
    for (idx_t i = 0; i < (size_t)dcnt; ++i) idx_of[divs[i]] = (int)i;

    /* --------------------------------------------------------------------- */
    /* 2. dynamic programming: dp[sum][lcm_index]  -> count of subsets */
    /*    dp is stored as a flat array of seq_t with stride dcnt              */
    /* --------------------------------------------------------------------- */

    /* Allocate one contiguous block: (n+1) * dcnt entries */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1) * (size_t)dcnt, sizeof(seq_t));
    if (!dp) return 0; /* allocation failure, should not happen */

    /* initial state: empty subset has sum 0 and lcm 1 */
    int idx_one = idx_of[1];
    dp[0 * dcnt + idx_one] = 1;

    /* process each divisor d in ascending order (distinct parts guarantee each used at most once) */
    for (idx_t di = 0; di < dcnt; ++di) {
        int d = (int)divs[di];
        for (idx_t sum = n; sum >= d; --sum) {
            seq_t *row_from = dp + (sum - d) * dcnt;
            seq_t *row_to   = dp + sum * dcnt;
            for (idx_t li = 0; li < dcnt; ++li) {
                seq_t cnt = row_from[li];
                if (!cnt) continue;
                /* compute new lcm of previous lcm and d */
                int prev = (int)divs[li];
                idx_t g = gcd_uint64((idx_t)prev, (idx_t)d);
                idx_t l = (prev / (int)g) * (idx_t)d;   /* l divides n */
                int lidx = idx_of[l];
                /* add to target state */
                row_to[lidx] += cnt;
            }
        }
    }

    /* answer: number of subsets summing to n with lcm exactly n */
    seq_t result = dp[n * dcnt + idx_of[n]];

    free(dp);
    free(idx_of);
    return result;
}
