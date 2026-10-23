/*
 * A197904  a(n) = ceiling((n+1/n)^5).
 * https://oeis.org/A197904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n + 1/n)^5) (OEIS A197904)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 10)
    {
        seq_t ns = (seq_t)n;
        seq_t base = ns * ns + (seq_t)1;

        /* base^5 */
        seq_t p = base;
        p = p * base;   /* ^2 */
        p = p * base;   /* ^3 */
        p = p * base;   /* ^4 */
        p = p * base;   /* ^5 */

        /* n^5 */
        seq_t q = ns;
        q = q * ns;     /* n^2 */
        q = q * ns;     /* n^3 */
        q = q * ns;     /* n^4 */
        q = q * ns;     /* n^5 */

        /* ceil(p / q) = (p + q - 1) / q */
        seq_t res = (p + q - (seq_t)1) / q;
        return res;
    }
    else
    {
        seq_t ns = (seq_t)n;
        seq_t n2 = ns * ns;
        seq_t n3 = n2 * ns;
        seq_t n5 = n3 * n2;                 /* n^5 */

        seq_t res = n5 + (seq_t)5 * n3 + (seq_t)10 * ns + (seq_t)1;
        return res;
    }
}
