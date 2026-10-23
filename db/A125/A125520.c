/*
 * A125520  a(n) is the maximal difference between two distinct n-digit
 *          numbers with the property that when one of them is typed into a
 *          calculator and rotated 180 degrees, the other one is seen.
 * https://oeis.org/A125520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1)=3, a(2)=75. For n≥3: if n is even, a(n) is the n‑digit
 *            number obtained by concatenating: 8, (n/2−2) copies of 9, the
 *            digit 8, (n/2−1) copies of 3, and the digit 5. If n is odd, a(n)
 *            is obtained by concatenating: 8, (⌊n/2⌋−1) copies of 9, the
 *            digit 2, (⌊n/2⌋−1) copies of 3, and the digit 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)75;

    seq_t res = (seq_t)8;          /* start with the leading 8 */
    idx_t half = n / (idx_t)2;     /* integer division */

    if ((n & ((idx_t)1)) == 0)     /* even n */
    {
        /* append (half-2) copies of 9 */
        if (half > 2)
        {
            for (idx_t i = 0; i < half - (idx_t)2; ++i)
                res = res * (seq_t)10 + (seq_t)9;
        }

        /* append the middle 8 */
        res = res * (seq_t)10 + (seq_t)8;

        /* append (half-1) copies of 3 */
        for (idx_t i = 0; i < half - (idx_t)1; ++i)
            res = res * (seq_t)10 + (seq_t)3;
    }
    else                          /* odd n */
    {
        /* append (half-1) copies of 9 */
        if (half > 0)
        {
            for (idx_t i = 0; i < half - (idx_t)1; ++i)
                res = res * (seq_t)10 + (seq_t)9;
        }

        /* append the middle 2 */
        res = res * (seq_t)10 + (seq_t)2;

        /* append (half-1) copies of 3 */
        for (idx_t i = 0; i < half - (idx_t)1; ++i)
            res = res * (seq_t)10 + (seq_t)3;
    }

    /* append the trailing 5 */
    res = res * (seq_t)10 + (seq_t)5;

    return res;
}
