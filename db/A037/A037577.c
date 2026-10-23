/*
 * A037577  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3.
 * https://oeis.org/A037577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(5^n / 3) Recurrence: a(1) = 1, a(2) = 8, a(3) = 41,
 *            a(n) = 5*a(n-1) + a(n-2) - 5*a(n-3) for n >= 4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases  */
    if (n == 0)
        return 0;          /* off‑set is 1; value for n=0 is unused but harmless */

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)8;
    if (n == 3)
        return (seq_t)41;

    /* Recurrence variables */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) for current n = 4  */
    seq_t a_nm2 = (seq_t)8;   /* a(n-2)                     */
    seq_t a_nm1 = (seq_t)41;  /* a(n-1)                     */
    const seq_t five = (seq_t)5;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = five * a_nm1 + a_nm2 - five * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
