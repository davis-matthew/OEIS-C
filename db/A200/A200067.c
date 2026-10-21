/*
 * A200067  Maximum sum of all products of absolute differences and distances
 *          between element pairs among the integer partitions of n.
 * https://oeis.org/A200067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - k - 1) * k * (k + 1) / 2, where k = max(0,
 *            floor((2*n - 1)/3)) (equivalently k = floor((2*n-1)/3)); For n =
 *            0 and n = 1 the value is 0
 * Range:     u8 n <= 15, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 or 1 yields 0 */
    if (n <= 1)
        return 0;

    /* compute k without unsigned overflow:
       k = floor((2*n-1)/3)   for n >= 2.  */
    idx_t q = n / 3;          /* floor division */
    idx_t r = n % 3;          /* remainder 0,1,2 */
    idx_t k;
    if (r == 0)
        k = 2 * q - 1;        /* n = 3q  -> k = 2q - 1  (q>=1 here) */
    else if (r == 1)
        k = 2 * q;            /* n = 3q+1 -> k = 2q    */
    else                     /* r == 2 */
        k = 2 * q + 1;        /* n = 3q+2 -> k = 2q+1  */

    /* (n - k - 1) is guaranteed non‑negative for n >= 2. */
    seq_t kk = (seq_t)k;     /* convert k to seq_t for arithmetic   */
    seq_t prod = kk * (kk + 1); /* kk*(kk+1)  */
    prod /= 2;                 /* divide by 2 safely                */
    seq_t mult = (seq_t)(n - kk - 1);
    return mult * prod;
}
