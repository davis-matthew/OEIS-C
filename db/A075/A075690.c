/*
 * A075690  a(n) = (n-1)*(n-2)^4 - A028294(n), for n > 4, with a(1) = a(2) =
 *          0, a(3) = 2, and a(4) = 48.
 * https://oeis.org/A075690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^4 + 19*n^3 - 632*n^2 + 2012*n - 1686)/6 for n ≥ 5;
 *            a(1) = 0, a(2) = 0, a(3) = 2, a(4) = 48
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 149, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n <= 4)
    {
        switch ((uint64_t)n)
        {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)2;
            case 4: return (seq_t)48;
        }
    }

    /* Compute the closed‑form polynomial for n ≥ 5. */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;        /* n^2   */
    seq_t n3 = n2 * N;       /* n^3   */
    seq_t n4 = n3 * N;       /* n^4   */

    seq_t num = (seq_t)11 * n4;                 /* 11·n^4   */
    num += (seq_t)19 * n3;                     /* +19·n^3   */
    num -= (seq_t)632 * n2;                    /* -632·n^2  */
    num += (seq_t)2012 * N;                    /* +2012·n   */
    num -= (seq_t)1686;                        /* -1686     */

    seq_t result = num / (seq_t)6;              /* divide by 6 */
    return result;
}
