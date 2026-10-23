/*
 * A123381  Values x of the solutions (x,y) of the Diophantine equation
 *          5*(X-Y)^4 - 16*X*Y = 0 with X >= Y.
 * https://oeis.org/A123381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123381: a(n) = 340*a(n-1) - 5798*a(n-2) + 340*a(n-3) - a(n-4)
   for n≥4 with a(0)=0, a(1)=40, a(2)=11664, a(3)=3733880. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)11664;
    if (n == 3) return (seq_t)3733880;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)40;
    seq_t a2 = (seq_t)11664;
    seq_t a3 = (seq_t)3733880;
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a4 = 340*a3 + 340*a1 - (5798*a2 + a0) to avoid
           negative intermediate values with unsigned seq_t. */
        seq_t t1 = ((seq_t)340) * a3 + ((seq_t)340) * a1;
        seq_t t2 = ((seq_t)5798) * a2 + a0;
        a4 = t1 - t2;

        /* Shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
