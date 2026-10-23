/*
 * A182445  a(0)=0, a(1)=1, for n>1, a(n) = a(n-2) + (a(n-1) mod n).
 * https://oeis.org/A182445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, for n>1: a(n)=a(n-2)+(a(n-1) mod n)
 * Range:     u8 n <= 40, u16 n <= 717, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;  /* a(n-2) */
    seq_t prev1 = (seq_t)1;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mod_val = prev1 % ((seq_t)i); /* a(n-1) mod n   */
        seq_t cur = prev2 + mod_val;        /* a(n)            */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
