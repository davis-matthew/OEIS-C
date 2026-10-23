/*
 * A157024  a(0)=1, a(n) = (3n-1)*3n*(3n+1)/2 for n>0.
 * https://oeis.org/A157024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = (3*n-1)*3*n*(3*n+1)/2
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute 3*n as a seq_t */
    seq_t three_n = (seq_t)3 * (seq_t)n;
    seq_t res;

    /* divide one of the three factors by 2 to keep intermediate
       values small and avoid overflow on small seq_t types */
    if ((three_n & (seq_t)1) == 0) {
        /* three_n is even */
        seq_t half = three_n / (seq_t)2;
        res = (three_n - (seq_t)1) * half * (three_n + (seq_t)1);
    } else if (((three_n - (seq_t)1) & (seq_t)1) == 0) {
        /* (three_n-1) is even */
        seq_t half = (three_n - (seq_t)1) / (seq_t)2;
        res = half * three_n * (three_n + (seq_t)1);
    } else {
        /* (three_n+1) must be even */
        seq_t half = (three_n + (seq_t)1) / (seq_t)2;
        res = (three_n - (seq_t)1) * three_n * half;
    }

    return res;
}
