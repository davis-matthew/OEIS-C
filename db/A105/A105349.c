/*
 * A105349  Characteristic sequence for the Pell numbers.
 * https://oeis.org/A105349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is a Pell number, 0 otherwise. Pell numbers
 *            satisfy P0 = 0, P1 = 1, Pk = 2*P_{k-1} + P_{k-2}
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two indices explicitly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    unsigned __int128 p0 = 0, p1 = 1;
    for (;;)
    {
        unsigned __int128 next = 2 * p1 + p0;   /* Pell recurrence */

        /* If the next Pell number exceeds n, n cannot be a Pell number */
        if (next > (unsigned __int128)n)
            return (seq_t)0;

        /* If it matches n, we found the Pell number */
        if (next == (unsigned __int128)n)
            return (seq_t)1;

        /* Advance to the next Pell number */
        p0 = p1;
        p1 = next;
    }
}
