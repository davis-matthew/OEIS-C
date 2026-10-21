/*
 * A058649  a(n) = 2^(n-4)*n*(n+1)*(n^2+5*n-2).
 * https://oeis.org/A058649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4) * n * (n+1) * (n^2 + 5*n - 2)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt   = (seq_t)n;                     /* convert index to seq_t */
    seq_t n1   = nt + 1;                       /* n+1  */
    seq_t n2   = nt * nt;                      /* n^2  */
    seq_t part = n2 + 5 * nt - 2;              /* n^2 + 5*n - 2 */
    seq_t base = nt * n1 * part;               /* n*(n+1)*(n^2+5*n-2) */

    if (n >= 4) {
        /* multiply by 2^(n-4) : left shift by n-4 */
        return base << (unsigned long long)(n - 4);
    } else {
        /* divide by 2^(4-n) : right shift by 4-n */
        return base >> (unsigned long long)(4 - n);
    }
}
