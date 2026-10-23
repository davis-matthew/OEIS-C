/*
 * A125445  Number of base 10 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*a(n-1) - a(n-2) - 8*a(n-3) for n>3 with a(0) = 1, a(1)
 *            = 10, a(2) = 98, a(3) = 946
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)98;
    if (n == 3) return (seq_t)946;

    seq_t a_nm3 = (seq_t)1;   /* a(0)   */
    seq_t a_nm2 = (seq_t)10;  /* a(1)   */
    seq_t a_nm1 = (seq_t)98;  /* a(2)   */
    seq_t a_n   = (seq_t)946; /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t temp = 10 * a_n - a_nm1 - (seq_t)8 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
        a_n   = temp;
    }
    return a_n;
}
