/*
 * A016173  Expansion of 1/((1-6*x)*(1-10*x)).
 * https://oeis.org/A016173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=16, a(n)=16*a(n-1)-60*a(n-2) for n>=2 (OEIS
 *            A016173)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)16;
    }

    /* iterative computation using only constant extra space */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)16;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = ((seq_t)16 * prev1) - ((seq_t)60 * prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
