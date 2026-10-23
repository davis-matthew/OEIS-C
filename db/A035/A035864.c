/*
 * A035864  Coordination sequence for A_38 lattice.
 * https://oeis.org/A035864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d=1..38} C(39,d) * C(n-1,d-1) * C(38-d+n, n) (n>0)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

/* Binomial coefficient computed multiplicatively.
   It works for all supported seq_t types (uint8_t, uint16_t, uint32_t,
   uint64_t, unsigned __int128, or big integer classes). */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n)            return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k)        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    seq_t total = 0;
    for (idx_t d = 1; d <= 38; ++d) {
        seq_t t = binom(39, d);                     /* C(39,d)      */
        t *= binom(n - 1, d - 1);                   /* C(n-1,d-1)   */
        t *= binom(38 - d + n, n);                 /* C(38-d+n,n)  */
        total += t;
    }
    return total;
}
