/*
 * A130859  1729-gonal numbers.
 * https://oeis.org/A130859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1727*n^2 - 1725*n)/2
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n == 1) return 1;
    if (n == 2) return 1729;
    if (n == 3) return 5184;

    /* Use the linear recurrence:
       a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3)   for n > 3   */
    seq_t a1 = 1;      /* a(1) */
    seq_t a2 = 1729;   /* a(2) */
    seq_t a3 = 5184;   /* a(3) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 * 3U - a2 * 3U + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
