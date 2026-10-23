/*
 * A198014  Number of isomorphism classes of nanocones with 3 pentagons and a
 *          nearsymmetric boundary of length n.
 * https://oeis.org/A198014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198014 from OEIS.
 * Recurrence derived from the generating function:
 *   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - 2*a(n-4) - a(n-5)
 *          + 2*a(n-6) + a(n-7) - a(n-8)   (n ≥ 9)
 * Initial values (offset 1):
 *   a(1)=2, a(2)=8, a(3)=18, a(4)=37, a(5)=63, a(6)=104,
 *   a(7)=153, a(8)=224
 */
seq_t a(idx_t n)
{
    static const seq_t base[8] = { (seq_t)2, (seq_t)8, (seq_t)18, (seq_t)37,
                                   (seq_t)63, (seq_t)104, (seq_t)153, (seq_t)224 };
    if (n <= 8)
        return base[n-1];

    /* Coefficients from the denominator of the generating function:
     *   q1 = -1, q2 = -2, q3 =  1, q4 =  2, q5 =  1,
     *   q6 = -2, q7 = -1, q8 =  1   */
    static const int coeff[8] = { -1, -2, 1, 2, 1, -2, -1, 1 };

    seq_t wnd[8];
    for (int k = 0; k < 8; ++k)
        wnd[k] = base[k];

    seq_t result = 0;

    for (idx_t i = 9; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 8; ++k) {
            seq_t val = wnd[8 - k];
            int c   = coeff[k-1];
            if (c > 0)
                pos += val * (seq_t)c;
            else if (c < 0)
                neg += val * (seq_t)(-c);
        }
        result = neg - pos;

        /* slide the window: discard oldest, add new */
        for (int j = 0; j < 7; ++j)
            wnd[j] = wnd[j+1];
        wnd[7] = result;
    }

    return result;
}
