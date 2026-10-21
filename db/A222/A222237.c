/*
 * A222237  In the number n, replace all (decimal) digits '3' with '7' and
 *          vice versa.
 * https://oeis.org/A222237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   replace each decimal digit '3' with '7' and vice versa
 * Range:     u8 n <= 229, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long tmp   = (unsigned long long)n; /* work as 64-bit for safety */
    unsigned long long res   = 0;                     /* result accumulator      */
    unsigned long long place = 1;                     /* 10^position             */

    while (tmp > 0)
    {
        unsigned long long d = tmp % 10ULL;           /* current decimal digit   */
        if (d == 3ULL) d = 7ULL;
        else if (d == 7ULL) d = 3ULL;
        res += d * place;                            /* accumulate digit        */
        place *= 10ULL;
        tmp  /= 10ULL;
    }
    return (seq_t)res;                                 /* cast back to seq_t      */
}
