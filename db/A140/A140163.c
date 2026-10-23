/*
 * A140163  a(1)=1, a(n) = a(n-1) + n^5 if n odd, a(n) = a(n-1) + n if n is
 *          even.
 * https://oeis.org/A140163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} (k^5 if k odd else k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside offset, safe guard */
    seq_t result = 1;              /* a(1) */
    if (n == 1) return result;
    for (idx_t i = 2; i <= n; ++i)
    {
        if ((i & 1) == 0)           /* even */
        {
            result += (seq_t)i;
        }
        else                        /* odd */
        {
            seq_t k  = (seq_t)i;
            seq_t k2 = k * k;
            seq_t k3 = k2 * k;
            seq_t k4 = k3 * k;
            seq_t k5 = k4 * k;
            result += k5;
        }
    }
    return result;
}
