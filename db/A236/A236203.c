/*
 * A236203  Interleave A005563(n), A028347(n).
 * https://oeis.org/A236203
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 4)/4 for n even; (n^2 + 2*n - 15)/4 for n odd (n
 *            >= 2)
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(2) is defined as 0 and the formula works for all n >= 2. */
    if (n < 2) return 0;  /* undefined offset; return 0 for safety */

    if ((n & 1ULL) == 0)    /* even n */
    {
        /* Let n = 2k.  Then a(n) = k^2 - 1. */
        seq_t k = (seq_t)(n >> 1);
        return k * k - 1;
    }
    else                    /* odd n */
    {
        /* Let n = 2k+1.  Then a(n) = k^2 + 2k - 3. */
        seq_t k = (seq_t)((n - 1) >> 1);
        return k * k + 2 * k - 3;
    }
}
