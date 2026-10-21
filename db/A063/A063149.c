/*
 * A063149  Composite numbers which in base 5 contain their largest proper
 *          factor as a substring.
 * https://oeis.org/A063149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 30*n – a(n‑1) for n>1, with a(1)=25. Closed form: a(n) =
 *            25 + 15*(n‑1) if n is odd a(n) = 35 + 15*(n‑2) if n is even
 *            (Both formulas are equivalent to the above recurrence.)
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for the sequence, but harmless */
    if (n & 1)                       /* odd n */
    {
        return (seq_t)25 + (seq_t)15 * (((seq_t)n) - 1);
    }
    else                             /* even n */
    {
        return (seq_t)35 + (seq_t)15 * (((seq_t)n) - 2);
    }
}
