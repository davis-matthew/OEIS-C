/*
 * A166863  a(1)= 1; a(2)= 5; thereafter a(n)= a(n-1) + a(n-2) + 5.
 * https://oeis.org/A166863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 5; a(1)=1, a(2)=5
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1; for n == 0 return 0 (undefined in OEIS context) */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)5;

    /* use a sliding window of the last two values */
    seq_t a_nm2 = (seq_t)1;   /* a(1)   */
    seq_t a_nm1 = (seq_t)5;   /* a(2)   */
    seq_t a_n   = 0;          /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a_n   = a_nm1 + a_nm2 + (seq_t)5;  /* recurrence */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
