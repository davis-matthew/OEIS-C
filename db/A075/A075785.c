/*
 * A075785  Number of ways to express 3/n as Egyptian fractions in just three
 *          terms.
 * https://oeis.org/A075785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct ordered triples * (a,b,c) with a<b<c
 *            and 1/a+1/b+1/c = 3/n * computed by enumerating possible 'a' and
 *            'b' with bounds derived * from 3ab > n(a+b) and c = n·a·b / (3ab
 *            – n(a+b))
 * Range:     u8 n <= 10, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* idx_t and seq_t are defined externally */
seq_t a(idx_t n)
{
    seq_t ans = 0;
    if (n < 1) return 0;

    /* loop over a: a must satisfy a > n/3 */
    idx_t start_a = (n / 3) + 1;           /* ceil(n/3 + ε)  */
    for (idx_t a_val = start_a; a_val <= n; ++a_val)
    {
        /* 3a – n > 0 because a > n/3 */
        idx_t denom = 3 * a_val - n;      /* positive */

        /* lower bound for b: b > n*a/(3a – n) and b > a */
        /* compute floor( n*a / denom ) */
        idx_t L = (n * a_val) / denom;
        idx_t b_min = (a_val + 1) > (L + 1) ? (a_val + 1) : (L + 1);

        /* upper bound for b: b <= 2*n*a/(3a – n)  */
        idx_t U = (2 * n * a_val) / denom;
        if (b_min > U) continue;

        for (idx_t b_val = b_min; b_val <= U; ++b_val)
        {
            /* d = 3ab – n(a+b) */
            __uint128_t d = (__uint128_t)3 * a_val * b_val
                           - (__uint128_t)n * (a_val + b_val);
            if (d == 0) continue;          /* should not happen */

            __uint128_t num = (__uint128_t)n * a_val * b_val;
            if (num % d != 0) continue;

            seq_t c_val = (seq_t)(num / d);
            if (c_val <= (seq_t)b_val) continue;  /* need a<b<c */

            /* one valid representation found */
            ++ans;
        }
    }

    return ans;
}
