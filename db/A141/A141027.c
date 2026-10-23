/*
 * A141027  a(n) = smallest number m which without its leftmost digit is equal
 *          to m/n (or 0 if no such number exists).
 * https://oeis.org/A141027
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m with m/n equal to m with its leftmost digit
 *            removed, 0 if none
 * Range:     u8 n <= 8, u16 n <= 64, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation based on the analytical condition: (n‑1) | d·10^k where d < n‑1, d∈[1,9] and
 * the minimal k gives the smallest m = n·(d·10^k/(n‑1)). */

#include <stdbool.h>

/* Helper: gcd for seq_t values */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: compute 10^e in seq_t */
static inline seq_t pow10_seq(int e)
{
    seq_t res = 1;
    while (e > 0) {
        res *= 10;
        --e;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Offset starts at n = 2; values for n < 2 are undefined, return 0 */
    if (n < 2) return 0;

    seq_t best = 0;                /* 0 means no solution found yet */

    seq_t n_minus = (seq_t)(n - 1); /* n-1 > 0 */

    for (int d = 1; d <= 9; ++d) {
        /* leftmost digit must be strictly less than n‑1 */
        if ((seq_t)d >= n_minus) continue;

        seq_t g = gcd_seq((seq_t)d, n_minus);
        seq_t t = n_minus / g;          /* t must be a product of only 2 and 5 */

        /* factor t into 2^e2 * 5^e5 */
        seq_t tmp = t;
        int e2 = 0, e5 = 0;
        while ((tmp & 1) == 0) {     /* tmp % 2 == 0 */
            tmp >>= 1;
            ++e2;
        }
        while (tmp % 5 == 0) {
            tmp /= 5;
            ++e5;
        }
        if (tmp != 1) continue;       /* contains other prime factors → skip */

        /* minimal k such that 10^k is divisible by t */
        int k = e2 > e5 ? e2 : e5;
        if (k == 0) continue;         /* k must be at least 1 to have a non‑zero r */

        /* compute r = d·10^k/(n‑1) and m = n·r */
        seq_t pow10 = pow10_seq(k);
        seq_t r = ((seq_t)d * pow10) / n_minus;
        if (r >= pow10) continue;     /* r must have at most k digits */

        seq_t m = (seq_t)n * r;

        if (best == 0 || m < best) best = m;
    }

    return best;   /* 0 if no such m exists */
}
