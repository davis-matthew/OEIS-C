/*
 * A121102  Catapolyoctagons (see Cyvin et al. for precise definition).
 * https://oeis.org/A121102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-4) - 30*a(n-3) + 6*a(n-1), with initial values
 *            a(1)=a(2)=a(3)=0, a(4)=4
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;
    if (n == 4)
        return 4;

    seq_t a1 = 0,   /* a(n-4) */
         a2 = 0,   /* a(n-3) */
         a3 = 0,   /* a(n-2)  unused in the recurrence, kept for clarity */
         a4 = 4;   /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_a = 25 * a1 + 6 * a4 - 30 * a2;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = new_a;
    }
    return a4;
}
