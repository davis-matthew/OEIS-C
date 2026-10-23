/*
 * A116921  a(n) = largest integer <= n/2 which is coprime to n.
 * https://oeis.org/A116921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the largest integer ≤ n/2 that is coprime to n.
 *            Simplification: a(1) = 0 a(2) = 1 For n ≥ 3: if n is odd: a(n) =
 *            (n - 1) / 2 else if n ≡ 0 (mod 4): a(n) = n/2 - 1 else (n ≡ 2
 *            (mod 4)): a(n) = n/2 - 2
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    if (n == 2)
        return 1;

    seq_t sn = (seq_t)n;          /* promote to seq_t */
    if (sn & 1)                   /* odd n */
        return (sn - 1) / 2;
    else
    {
        if ((sn & 3) == 0)       /* n mod 4 == 0 */
            return sn / 2 - 1;
        else                     /* n mod 4 == 2 */
            return sn / 2 - 2;
    }
}
