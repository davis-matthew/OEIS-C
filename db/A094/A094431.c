/*
 * A094431  a(n) = left term in M^n * [1 0 0], where M = the 3 X 3 matrix [1
 *          -1 0 / -1 4 -3 / 0 -3 3].
 * https://oeis.org/A094431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094431.
   a(0) = 1, a(1) = 1, a(2) = 2
   a(n) = 8*a(n-1) - 9*a(n-2)   for n >= 3
   The recurrence yields only positive values after n = 2, so unsigned
   arithmetic can be used safely once the initial values are set. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2)   */
    seq_t a_nm1 = (seq_t)2;   /* a(n-1)   */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i)
    {
        ai = 8 * a_nm1 - 9 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }
    return a_nm1;
}
