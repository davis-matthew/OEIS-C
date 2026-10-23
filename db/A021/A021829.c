/*
 * A021829  Expansion of 1/((1-x)(1-4x)(1-6x)(1-10x)).
 * https://oeis.org/A021829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(n) = 21*a(n-1) - 144*a(n-2) + 364*a(n-3) - 240*a(n-4)
 * initial values: a(0)=1, a(1)=21, a(2)=297, a(3)=3577 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)297;
    if (n == 3) return (seq_t)3577;

    seq_t prev4 = (seq_t)1;
    seq_t prev3 = (seq_t)21;
    seq_t prev2 = (seq_t)297;
    seq_t prev1 = (seq_t)3577;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t curr = ((seq_t)21 * prev1) + ((seq_t)364 * prev3);
        curr -= ((seq_t)144 * prev2);
        curr -= ((seq_t)240 * prev4);
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
