/*
 * A014936  a(1)=1, a(n) = n*19^(n-1) + a(n-1).
 * https://oeis.org/A014936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*a(n-1) - 361*a(n-2) + 1 with a(1)=1, a(2)=39
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly */
    if (n == 1)
        return 1;
    if (n == 2)
        return 39;

    /* Iteratively compute using the recurrence relation */
    seq_t a_nm2 = 1;   /* a(1) */
    seq_t a_nm1 = 39;  /* a(2) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = (seq_t)38 * a_nm1 - (seq_t)361 * a_nm2 + 1;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
