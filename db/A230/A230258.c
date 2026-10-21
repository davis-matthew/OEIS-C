/*
 * A230258  The number of multinomial coefficients over partitions with value
 *          equal to 10.
 * https://oeis.org/A230258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            floor((n-1)/9)+floor((n-1)/10)-floor(n/10)+floor((n-1)/5)+floor((n-1)/6)-floor(n/6)+floor((n+1)/6)-floor(n/5)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) should be the first term, but the formula works for all n>=1 */
    if (n == 0) return 0;                 /* safety for n=0, though not used */

    long long sum = 0;
    idx_t nm1 = n - 1;

    sum += (long long)(nm1 / 9);
    sum += (long long)(nm1 / 10);
    sum -= (long long)(n / 10);
    sum += (long long)(nm1 / 5);
    sum += (long long)(nm1 / 6);
    sum -= (long long)(n / 6);
    sum += (long long)((n + 1) / 6);
    sum -= (long long)(n / 5);

    return (seq_t)sum;
}
