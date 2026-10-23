/*
 * A167238  Number of ways to partition a 2*n X 2 grid into 4 connected
 *          equal-area regions.
 * https://oeis.org/A167238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)-2*a(n-2)-2*a(n-3)+3*a(n-4)-a(n-5) Initial
 *            values: a(1)=1, a(2)=5, a(3)=11, a(4)=25, a(5)=45
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* below offset, undefined */
    if (n <= 5) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)5;
            case 3: return (seq_t)11;
            case 4: return (seq_t)25;
            case 5: return (seq_t)45;
        }
    }

    seq_t a1 = (seq_t)1, a2 = (seq_t)5, a3 = (seq_t)11, a4 = (seq_t)25, a5 = (seq_t)45, an = 0;
    for (idx_t i = 6; i <= n; ++i) {
        an = ((seq_t)3) * a5 - ((seq_t)2) * a4 - ((seq_t)2) * a3 + ((seq_t)3) * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return an;
}
