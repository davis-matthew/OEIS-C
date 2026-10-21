/*
 * A084874  Number of (k,m,n)-antichains of multisets with k=3 and m=2.
 * https://oeis.org/A084874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9^n - 2*6^n + 3^n)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow9 = pow_seq((seq_t)9, n);
    seq_t pow6 = pow_seq((seq_t)6, n);
    seq_t pow3 = pow_seq((seq_t)3, n);

    seq_t temp = pow9 - (pow6 * (seq_t)2) + pow3;
    return temp / (seq_t)2;
}
