/*
 * A091739  Third column (k=7) sequence of array A090216 ((5,5)-Stirling2)
 *          divided by 600.
 * https://oeis.org/A091739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (21*2520^n - 12*720^n + 120^n) / 10
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* fast exponentiation for seq_t (works for both fixed- and arbitrary-size builds) */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p1 = pow_seq_t((seq_t)2520, n);   /* 2520^n   */
    seq_t p2 = pow_seq_t((seq_t)720,  n);   /* 720^n    */
    seq_t p3 = pow_seq_t((seq_t)120,  n);   /* 120^n    */

    seq_t numerator = (seq_t)21 * p1 - (seq_t)12 * p2 + p3;
    return numerator / (seq_t)10;
}
