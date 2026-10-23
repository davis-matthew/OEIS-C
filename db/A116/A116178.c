/*
 * A116178  Stewart's choral sequence: a(3n) = 0, a(3n-1) = 1, a(3n+1) = a(n).
 * https://oeis.org/A116178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3*n) = 0, a(3*n-1) = 1, a(3*n+1) = a(n)
 * Range:     u8 n <= 9636, u16 n <= 9636, u32 n <= 9636, u64 n <= 9636,
 *            u128 n <= 9636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Stewart's choral sequence at index n. */
seq_t a(idx_t n)
{
    for (;;)
    {
        switch (n % 3)
        {
            case 0:  /* n = 3*k */
                return 0;
            case 2:  /* n = 3*k + 2 */
                return 1;
            case 1:  /* n = 3*k + 1 */
                n = (n - 1) / 3;
                break;
        }
    }
}
