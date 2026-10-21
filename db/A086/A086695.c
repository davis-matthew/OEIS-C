/*
 * A086695  a(n) = 100^n - 10^n - 1.
 * https://oeis.org/A086695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^(2*n) - 10^n - 1 Recurrence (valid for n>=4): a(n) =
 *            111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) Base values: a(1) = 89,
 *            a(2) = 9899, a(3) = 998999
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle values below the offset – arbitrary choice */
    if (n == 0) return (seq_t)0;

    /* Base terms */
    seq_t t1 = (seq_t)89;      /* a(1) */
    if (n == 1) return t1;

    seq_t t2 = (seq_t)9899;    /* a(2) */
    if (n == 2) return t2;

    seq_t t3 = (seq_t)998999;  /* a(3) */
    if (n == 3) return t3;

    /* Compute iteratively using the recurrence */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = 111 * t3 - 1110 * t2 + 1000 * t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }

    return t3;
}
