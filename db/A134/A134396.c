/*
 * A134396  A007318 * A000125.
 * https://oeis.org/A134396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 52, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134396 (OEIS):
   a(n) = (2^(n-4) * (48 + 20*n + 3*n^2 + n^3)) / 3   for n ≥ 4
   a(0) = 1,  a(1) = 3,  a(2) = 9,  a(3) = 27.      */
/* Recurrence (for n > 3): a(n) = 8*a(n-1) - 24*a(n-2) + 32*a(n-3) - 16*a(n-4). */

seq_t a(idx_t n)
{
    /* handle the first four defined values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)27;

    /* compute for n ≥ 4 using the explicit formula */
    seq_t nn = (seq_t)n;          /* cast the index once */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */

    /* 48 + 20*n + 3*n^2 + n^3  */
    seq_t t = (seq_t)48
            + (seq_t)20 * nn
            + (seq_t)3  * n2
            + n3;

    /* divide by 3 (exact) */
    seq_t t_div3 = t / (seq_t)3;

    /* 2^(n-4)  */
    seq_t pow2 = ((seq_t)1) << (n - 4);

    /* multiply after division to keep intermediates small */
    return t_div3 * pow2;
}
