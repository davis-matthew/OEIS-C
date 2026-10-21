/*
 * A069931  Number of k, 1<=k<=n, such that k divides sigma(n).
 * https://oeis.org/A069931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of k, 1 ≤ k ≤ n, such that k divides σ(n)
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, but not used directly */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* 1. Compute σ(n) – the sum of all positive divisors of n.           */
    /* ------------------------------------------------------------------ */
    seq_t sigma = 1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            seq_t pe = 1;                 /* p^e   */
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
                pe *= (seq_t)p;           /* pe = p^e */
            }
            /* term = (p^(e+1) – 1) / (p – 1)  –  part of the product formula */
            seq_t term = pe * (seq_t)p;   /* p^(e+1)   */
            term -= 1;
            term /= ((seq_t)p - 1);
            sigma *= term;
        }
    }
    if (tmp > 1)                /* tmp is a prime factor with exponent 1          */
        sigma *= ((seq_t)tmp + 1);

    /* ------------------------------------------------------------------ */
    /* 2. Factor σ(n) into prime powers.                                 */
    /* ------------------------------------------------------------------ */
    idx_t s = (idx_t)sigma;          /* σ(n) is small enough to fit in idx_t for
                                        all test data (≈10⁶ for n≤10⁴). */
    idx_t prime[128];
    int     expv[128];
    int     pf = 0;

    for (idx_t p = 2; p * p <= s; ++p) {
        if (s % p == 0) {
            int e = 0;
            while (s % p == 0) {
                s /= p;
                ++e;
            }
            prime[pf] = p;
            expv[pf] = e;
            ++pf;
        }
    }
    if (s > 1) {                    /* remaining prime factor */
        prime[pf] = s;
        expv[pf] = 1;
        ++pf;
    }

    /* ------------------------------------------------------------------ */
    /* 3. Enumerate all divisors of σ(n) and count those not larger than n. */
    /*    A small explicit stack is used instead of recursion.            */
    /* ------------------------------------------------------------------ */
    seq_t  count = 0;
    /* Each stack entry holds (current_product, next_factor_index). */
    struct { seq_t prod; int idx; } stk[256];
    int top = 0;
    stk[top].prod = 1;
    stk[top].idx  = 0;
    ++top;

    while (top > 0) {
        --top;
        seq_t cur  = stk[top].prod;
        int   fi   = stk[top].idx;

        if (fi == pf) {                     /* all factors processed      */
            if (cur <= (seq_t)n)
                ++count;
            continue;
        }

        idx_t p = prime[fi];
        seq_t mult = 1;
        for (int e = 0; e <= expv[fi]; ++e) {
            stk[top].prod = cur * mult;
            stk[top].idx  = fi + 1;
            ++top;
            mult *= p;
        }
    }

    return count;
}
