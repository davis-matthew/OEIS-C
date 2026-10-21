/*
 * A084860  Expansion of (1 - 2*x + 2*x^2 - x^3)/(1 - 2*x)^2.
 * https://oeis.org/A084860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=2, a(2)=6
 * For n>=3: a(n)=2*a(n-1)+3*2^(n-3)
 * This recurrence implements OEIS sequence A084860. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    seq_t prev = (seq_t)6;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t inc = ((seq_t)3) << (i - 3);   /* 3 * 2^(i-3) */
        prev = (prev << 1) + inc;            /* 2*prev + inc   */
    }
    return prev;
}
