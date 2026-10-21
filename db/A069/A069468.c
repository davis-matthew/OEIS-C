/*
 * A069468  Number of ways writing n! as a product of some other numbers which
 *          has no digits equal to 1.
 * https://oeis.org/A069468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) counts the number of sequences of decimal digits 2–9
 * (no digit 1 allowed) whose product equals n!.  The order of the
 * digits matters.  The value is 0 for n ≥ 11 because n! then contains
 * a prime factor ≥ 11 which cannot be expressed as a product of digits
 * 2–9.
 *
 * Algorithm: dynamic programming over the prime exponents of the
 * factorial.  For every exponent state (e₂,e₃,e₅,e₇) we count how many
 * digit sequences reach that state.  Each digit contributes its
 * prime exponents; we add that contribution to obtain new states.
 * The state space is at most 9×5×3×2 = 270 for n ≤ 10, so the routine
 * runs in negligible time.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper: exponent of prime p in n! */
static inline int v_p_factorial(idx_t n, int p)
{
    int sum = 0;
    while (n)
    {
        n /= p;
        sum += n;
    }
    return sum;
}

/* digit exponents for digits 2..9 */
static const int exp2[8] = {1,0,2,0,1,0,3,0}; /* 2,3,4,5,6,7,8,9 */
static const int exp3[8] = {0,1,0,0,1,0,0,2};
static const int exp5[8] = {0,0,0,1,0,0,0,0};
static const int exp7[8] = {0,0,0,0,0,1,0,0};

seq_t a(idx_t n)
{
    /* n must be ≥ 1 */
    if (n == 1) return 0;          /* 1! = 1 has no valid representation */
    if (n > 10) return 0;          /* factorial contains prime ≥ 11 */

    /* prime exponents of n! for primes 2,3,5,7 */
    const int t2 = v_p_factorial(n, 2);
    const int t3 = v_p_factorial(n, 3);
    const int t5 = v_p_factorial(n, 5);
    const int t7 = v_p_factorial(n, 7);

    /* dimensions of DP table */
    const int d2 = t2 + 1;
    const int d3 = t3 + 1;
    const int d5 = t5 + 1;
    const int d7 = t7 + 1;

    /* total number of states */
    const size_t total = (size_t)d2 * d3 * d5 * d7;

    /* allocate state array */
    seq_t *dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!dp) return 0;  /* allocation failure – return 0 as safe default */

    /* initialise to zero */
    memset(dp, 0, total * sizeof(seq_t));

    /* index function: map 4‑D exponents to 1‑D array index */
    #define IDX(e2,e3,e5,e7) ((((size_t)(e2) * d3 + (e3)) * d5 + (e5)) * d7 + (e7))

    /* seed: product of no digits (empty product) */
    dp[IDX(0,0,0,0)] = 1;

    /* dynamic programming: add each digit as a step */
    for (int e2 = 0; e2 <= t2; ++e2)
    {
        for (int e3 = 0; e3 <= t3; ++e3)
        {
            for (int e5 = 0; e5 <= t5; ++e5)
            {
                for (int e7 = 0; e7 <= t7; ++e7)
                {
                    seq_t cur = dp[IDX(e2,e3,e5,e7)];
                    if (!cur) continue;           /* no way to reach this state */

                    /* try all digits 2..9 */
                    for (int k = 0; k < 8; ++k)
                    {
                        const int ne2 = e2 + exp2[k];
                        if (ne2 > t2) continue;
                        const int ne3 = e3 + exp3[k];
                        if (ne3 > t3) continue;
                        const int ne5 = e5 + exp5[k];
                        if (ne5 > t5) continue;
                        const int ne7 = e7 + exp7[k];
                        if (ne7 > t7) continue;

                        dp[IDX(ne2,ne3,ne5,ne7)] += cur;
                    }
                }
            }
        }
    }

    /* final result state */
    seq_t result = dp[IDX(t2,t3,t5,t7)];
    free(dp);
    return result;
}
