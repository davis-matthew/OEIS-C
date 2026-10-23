/*
 * A027799  a(n) = 91*(n+1)*binomial(n+3,14)/3.
 * https://oeis.org/A027799
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1,12)*C(n+3,3) (offset 11)
 * Range:     u8 none, u16 n <= 13, u32 n <= 22, u64 n <= 85, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t small_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * ((seq_t)(n - k + i));
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 11) return 0;                  /* not defined below offset */
    seq_t t1 = small_binom(n + 1, 12);
    seq_t t2 = small_binom(n + 3, 3);
    return t1 * t2;
}
