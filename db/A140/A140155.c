/*
 * A140155  a(1)=1, a(n)=a(n-1)+n^3 if n odd, a(n)=a(n-1)+ n^4 if n is even.
 * https://oeis.org/A140155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 133, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   a(n) = a(n‑1) + (n odd ? n³ : n⁴)        for n ≥ 2   (OEIS A140155) */
seq_t a(idx_t n)
{
    if (n == 0)          /* below offset – not used, but defined safely */
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk      = (seq_t)k;
        seq_t kk2     = kk * kk;              /* k²  */
        seq_t kk3     = kk2 * kk;             /* k³  */
        seq_t kk4 = (k & 1) ? kk3 : kk2 * kk2; /* k⁴ if even, else k³ */

        sum += kk4;
    }
    return sum;
}
