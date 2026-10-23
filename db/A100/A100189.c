/*
 * A100189  Equatorial structured meta-anti-diamond numbers, the n-th number
 *          from an equatorial structured n-gonal anti-diamond number
 *          sequence.
 * https://oeis.org/A100189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+3,4) + C(n+2,4) + 7*C(n+1,4) + 7*C(n,4)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 283, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(k,4) = k*(k-1)*(k-2)*(k-3)/24 (OEIS A100189) */

static inline seq_t binom4(idx_t k)
{
    if (k < 4) return 0;
    seq_t kk = (seq_t)k;
    seq_t prod = kk * (kk - 1);
    prod *= (kk - 2);
    prod *= (kk - 3);
    return prod / (seq_t)24;
}

seq_t a(idx_t n)
{
    seq_t res = binom4(n + 3);
    res += binom4(n + 2);
    res += ((seq_t)7) * binom4(n + 1);
    res += ((seq_t)7) * binom4(n);
    return res;
}
