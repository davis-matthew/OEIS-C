/*
 * A069915  Sum of (1+phi)-divisors of n (cf. A061389).
 * https://oeis.org/A069915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = Π_{p^e‖n} (1 + Σ_{1≤k≤e, gcd(k,e)=1} p^k) (multiplicative
 *            definition)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factors n by trial division and computes the corresponding factor for each prime power. */

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 1)        /* offset 1: a(1)=1 */
        return 1;

    idx_t remaining = n;
    seq_t result = 1;

    for (idx_t p = 2; p <= remaining / p; ++p) {
        if (p > 2 && (p & 1) == 0)        /* skip even numbers after 2 */
            continue;

        if (remaining % p == 0) {
            int e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }

            /* Compute a(p^e): 1 + Σ p^k for gcd(k, e) = 1 */
            seq_t term = 1;
            seq_t p_pow = (seq_t)p;
            for (int k = 1; k <= e; ++k) {
                if (gcd_idx((idx_t)k, (idx_t)e) == 1)
                    term += p_pow;
                p_pow *= (seq_t)p;
            }

            result *= term;
        }
    }

    /* If a prime factor > sqrt(original n) remains */
    if (remaining > 1) {
        seq_t term = 1 + (seq_t)remaining; /* e = 1, so only p^1 contributes */
        result *= term;
    }

    return result;
}
