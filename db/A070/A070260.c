/*
 * A070260  Third diagonal of triangle defined in A051537.
 * https://oeis.org/A070260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)/4 if n is even, else a(n) = n*(n+2)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses only integer arithmetic on the supplied types seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* Convert index to seq_t and compute according to parity. */
    if ((n & 1) == 0)   /* even */
        return (seq_t)n * ((seq_t)n + (seq_t)2) / (seq_t)4;
    else                /* odd */
        return (seq_t)n * ((seq_t)n + (seq_t)2);
}
