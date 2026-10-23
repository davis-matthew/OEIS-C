/*
 * A151787  a(1)=1; for n > 1, a(n)=3*2^{wt(n-1)-1}.
 * https://oeis.org/A151787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^{wt(n-1)-1} for n>1, a(1) = 1; wt(x) denotes the
 *            binary (Hamming) weight of x
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    /* Compute the popcount of n-1 (binary weight).  */
    uint64_t m = (uint64_t)(n - (idx_t)1);
    int w = __builtin_popcountll(m);   /* w >= 1 for n > 1 */

    /* a(n) = 3 * 2^(w-1) */
    seq_t power_of_two = ((seq_t)1) << (w - 1);
    return ((seq_t)3) * power_of_two;
}
