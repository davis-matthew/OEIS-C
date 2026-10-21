/*
 * A051638  a(n) = Sum_{k=0..n} (C(n,k) mod 3).
 * https://oeis.org/A051638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} (C(n,k) mod 3) Closed form: write n in base
 *            3 with r digits equal to 1 and s digits equal to 2, then a(n) =
 *            2^{r-1} * (3^{s+1} - 1) (interpreted as integer division by 2
 *            when r=0)
 * Range:     u8 n <= 240, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t r = 0;          /* count of digits equal to 1 */
    idx_t s = 0;          /* count of digits equal to 2 */

    /* Scan base-3 digits of n */
    while (temp != 0)
    {
        idx_t d = temp % 3;
        if (d == 1) ++r;
        else if (d == 2) ++s;
        temp /= 3;
    }

    /* Compute 3^(s+1)  (which is always odd) */
    seq_t pow3 = (seq_t)3;               /* start with 3^1 */
    for (idx_t i = 0; i < s; ++i)
        pow3 *= (seq_t)3;                /* multiply (s times) → 3^(s+1) */

    /* Compute 2^r  (safe for r=0) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < r; ++i)
        pow2 *= (seq_t)2;

    /* a(n) = (pow3 - 1) * pow2 / 2   (pow3 -1 is even) */
    seq_t result = (pow3 - (seq_t)1) * pow2;
    result /= (seq_t)2;

    return result;
}
