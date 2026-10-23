/*
 * A018633  Divisors of 764.
 * https://oeis.org/A018633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 764 in ascending order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divs[6];
    static int initialized = 0;
    if (!initialized)
    {
        idx_t i = 0;
        for (idx_t e191 = 0; e191 <= 1; ++e191)
        {
            for (idx_t e2 = 0; e2 <= 2; ++e2)
            {
                divs[i++] = ((seq_t)1 << e2) * ((seq_t)(e191 ? 191 : 1));
            }
        }
        initialized = 1;
    }
    if (n >= 1 && n <= 6)
        return divs[n - 1];
    return 0;
}
