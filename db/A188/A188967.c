/*
 * A188967  Zero-one sequence based on (3n-2): a(A016777(k))=a(k);
 *          a(A007494(k))=1-a(k); a(1)=0.
 * https://oeis.org/A188967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; * Recurrence: for n > 1 * if n ≡ 1 (mod 3) : a(n) =
 *            a((n + 2) / 3) * else : a(n) = 1 – a(n – ⌊(n + 2) / 3⌋) * The
 *            function iteratively follows this rule, counting the number of *
 *            “else” steps, which toggles the value
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;            /* a(1) = 0 */
    while (n > 1)
    {
        if (n % 3 == 1)       /* n is of the form 3k–2 */
        {
            n = (n + 2) / 3;  /* move to a(k) without flipping */
        }
        else                 /* n is not of the form 3k–2 */
        {
            val ^= 1;        /* flip because a(n) = 1 – a(k) */
            idx_t fl = (n + 2) / 3;
            n = n - fl;     /* k = n – ⌊(n + 2)/3⌋ */
        }
    }
    return val;
}
