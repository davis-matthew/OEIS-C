/*
 * A207643  a(n) = 1 + (n-1) + (n-1)*[n/2-1] + (n-1)*[n/2-1]*[n/3-1] +
 *          (n-1)*[n/2-1]*[n/3-1]*[n/4-1] +... for n>0 with a(0)=1, where [x]
 *          = floor(x).
 * https://oeis.org/A207643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 + Sum_{k=1..⌊n/2⌋} Π_{j=1..k} ⌊(n‑j)/j⌋
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 74, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(0) = a(1) = 1 */

    seq_t sum  = 1;                 /* the leading 1 in the definition */
    seq_t prod = 1;                 /* running product for each k */

    for (idx_t k = 1; k <= n / 2; ++k) {
        idx_t term = (n - k) / k;   /* floor((n-k)/k) */
        prod *= (seq_t)term;        /* update product */
        sum += prod;                /* add to the sum */
    }

    return sum;
}
