/*
 * A133998  Number of isomers of the soccerball C_60H_n Buckminster fullerene.
 * https://oeis.org/A133998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (1/60) * ( C(60,n) + 24*C(12, n/5) + 20*C(20, n/3) +
 *            15*C(30, n/2) )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n > 60ULL) return 0;
    seq_t res = 0;
    res += binom(60, n);
    if ((n % 5ULL) == 0ULL) res += 24 * binom(12, n / 5);
    if ((n % 3ULL) == 0ULL) res += 20 * binom(20, n / 3);
    if ((n % 2ULL) == 0ULL) res += 15 * binom(30, n / 2);
    res /= 60;
    return res;
}
