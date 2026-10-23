/*
 * A173099  a(1)=1, a(2)=3, a(n)=2*a(n-2)*a(n-1)-a(n-2)-a(n-1).
 * https://oeis.org/A173099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 3, a(n) = 2*a(n-2)*a(n-1) - a(n-2) - a(n-1) for
 *            n >= 3
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset of the sequence (a(1) is the first term). */
    if (n == 0) return 0;          /* undefined for n < 1, return 0 for safety */

    if (n == 1) return 1;          /* a(1) */
    if (n == 2) return 3;          /* a(2) */

    /* Iteratively compute the recurrence for n >= 3. */
    seq_t a_nm2 = 1; /* a(n-2), initially a(1) */
    seq_t a_nm1 = 3; /* a(n-1), initially a(2) */
    seq_t an = 0;   /* working variable for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = 2 * a_nm2 * a_nm1 - a_nm2 - a_nm1;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
