/*
 * A090300  a(n) = 14*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 14.
 * https://oeis.org/A090300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 14*a(n-1) + a(n-2) with a(0) = 2 and a(1) = 14
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial base cases */
    if (n == 0) {
        return 2;
    }
    if (n == 1) {
        return 14;
    }

    /* Compute the recurrence iteratively */
    seq_t a0 = 2;      /* a(n-2) */
    seq_t a1 = 14;     /* a(n-1) */
    seq_t a2;          /* a(n)   */

    for (idx_t i = 2; i <= n; ++i) {
        a2 = a1 * (seq_t)14 + a0;
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
