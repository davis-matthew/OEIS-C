/*
 * A065653  a(0) = 0, a(1) = 1, a(n) = a(n-2)*a(n-2) + 2 for n > 1.
 * https://oeis.org/A065653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A065653:
   a(0) = 0, a(1) = 1,
   a(n) = a(n-2)^2 + 2        for n > 1
*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a_nm2 = 0, a_nm1 = 1, cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) from a(i-2) */
        cur = a_nm2 * a_nm2 + 2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
