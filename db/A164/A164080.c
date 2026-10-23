/*
 * A164080  Perfect squares one less than a triangular number.
 * https://oeis.org/A164080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) with initial values
 *            a(1)=0, a(2)=9, a(3)=324
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) starts at index 1. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)324;

    seq_t prev3 = (seq_t)0;   /* a(n-3) */
    seq_t prev2 = (seq_t)9;   /* a(n-2) */
    seq_t prev1 = (seq_t)324; /* a(n-1) */
    seq_t curr;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        curr = (seq_t)35 * prev1 - (seq_t)35 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
