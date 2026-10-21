/*
 * A098790  a(n) = 2*a(n-1) + a(n-2) + 1, a(0) = 1, a(1) = 2.
 * https://oeis.org/A098790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 1, a(0) = 1, a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two seed values directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* For all n >= 2 compute iteratively in O(n) time */
    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)2; /* a(1) */
    seq_t next;
    for (idx_t i = 2; i <= n; ++i) {
        next = (seq_t)2 * a_nm1 + a_nm2 + (seq_t)1;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
