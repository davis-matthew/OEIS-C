/*
 * A061099  Squares with digital root 1.
 * https://oeis.org/A061099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (m(n))^2 where m(n) = 9 * floor((n-1)/2) + ( (n-1) mod 2
 *            ? 8 : 1 ) and indices start at 1 (a(1)=1). This generates
 *            squares of numbers ≡ 1 or 8 (mod 9), which are the squares whose
 *            digital root is 1
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)             /* undefined part; not used by the tests */
        return 0;

    idx_t n0 = n - 1;        /* zero‑based index 0,1,2,... */
    idx_t k  = n0 >> 1;      /* floor(n0/2) */
    seq_t  m;

    /* n0 even -> m = 9*k + 1,  n0 odd  -> m = 9*k + 8  */
    if ((n0 & 1) == 0)
        m = (seq_t)9 * (seq_t)k + 1;
    else
        m = (seq_t)9 * (seq_t)k + 8;

    return m * m;           /* square of m */
}
