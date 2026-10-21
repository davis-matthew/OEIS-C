/*
 * A091570  Sum of odd proper divisors of n. Sum of the odd divisors of n that
 *          are less than n.
 * https://oeis.org/A091570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the odd proper divisors of n Algorithm: let m be
 *            the odd part of n (remove all factors of 2). Compute σ(m), the
 *            sum of all divisors of m. If n is odd, subtract n from σ(m);
 *            otherwise the result is σ(m)
 * Range:     u8 n <= 314, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, defensive */
    /* 1. Remove all powers of 2 from n to obtain the odd part m. */
    uint64_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;

    /* 2. Compute σ(m) by factorising m with trial division. */
    seq_t sigma = 1;               /* σ(m) starts as 1 */
    uint64_t temp = m;

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            /* compute contribution for the prime power p^k */
            seq_t contrib = 1;     /* 1 + p + p^2 + ... + p^k */
            seq_t term = 1;
            while (temp % p == 0) {
                temp /= p;
                term *= (seq_t)p;      /* next power of p */
                contrib += term;       /* add p^i */
            }
            sigma *= contrib;          /* multiply into σ(m) */
        }
    }

    /* 3. Remaining prime factor, if any. */
    if (temp > 1) {
        sigma *= (1 + (seq_t)temp);     /* contribution = 1 + prime */
    }

    /* 4. Adjust for proper divisors. */
    if (n & 1ULL) {
        return sigma - (seq_t)n;         /* subtract n itself if odd */
    } else {
        return sigma;                    /* all odd divisors are proper */
    }
}
