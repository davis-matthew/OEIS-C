/*
 * A094248  Consider 3 X 3 matrix M = [0 1 0 / 0 0 1 / 5 2 0]; a(n) = the
 *          center term in M^n * [1 1 1].
 * https://oeis.org/A094248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(2) = 7 a(3) = 7 a(n) = 2*a(n‑2) + 5*a(n‑3) for n >= 4
 *            The recurrence is valid for all n >= 1. The implementation
 *            iterates in O(n) time and uses only constant additional space
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1 – lower indexes are irrelevant. */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)7;
    if (n == (idx_t)3) return (seq_t)7;

    seq_t a_nm3 = (seq_t)1;   /* a(n‑3)   */
    seq_t a_nm2 = (seq_t)7;   /* a(n‑2)   */
    seq_t a_nm1 = (seq_t)7;   /* a(n‑1)   */
    seq_t cur;                /* a(n)     */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        cur     = (seq_t)2 * a_nm2 + (seq_t)5 * a_nm3;
        a_nm3   = a_nm2;
        a_nm2   = a_nm1;
        a_nm1   = cur;
    }
    return cur;
}
