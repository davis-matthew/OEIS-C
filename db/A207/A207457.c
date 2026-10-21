/*
 * A207457  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 16*a(n-2) + 5*a(n-3) - 25*a(n-4), a(1)=12,
 *            a(2)=144, a(3)=396, a(4)=2460
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)144;
    if (n == 3) return (seq_t)396;
    if (n == 4) return (seq_t)2460;

    seq_t a_n_minus_4 = (seq_t)12;   /* a(n-4)  */
    seq_t a_n_minus_3 = (seq_t)144;  /* a(n-3)  */
    seq_t a_n_minus_2 = (seq_t)396;  /* a(n-2)  */
    seq_t a_n_minus_1 = (seq_t)2460; /* a(n-1)  */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = a_n_minus_1
                     + (seq_t)16 * a_n_minus_2
                     + (seq_t)5  * a_n_minus_3
                     - (seq_t)25  * a_n_minus_4;
        a_n_minus_4 = a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = next;
    }
    return a_n_minus_1;
}
