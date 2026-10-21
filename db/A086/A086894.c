/*
 * A086894  a(n) = (A000522(2*n) + 1)/2.
 * https://oeis.org/A086894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + (2*n)! * Sum_{k=0..2*n} 1/k!)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = 2 * n;          /* m = 2n */
    seq_t t = 1;              /* current term = m! / k! */
    seq_t sum = 0;            /* running sum of the terms */

    for (idx_t k = m; ; --k) {
        sum += t;                          /* add term m!/k! */
        if (k == 0) break;                  /* finished when k==0 */
        t *= (seq_t)k;                      /* compute next term: t *= k */
    }

    /* Now sum = Σ_{k=0..m} m!/k!.  Result is (sum + 1)/2. */
    return (sum + 1) / 2;
}
