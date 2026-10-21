/*
 * A064197  a(n) = 27*(n-1)*(n-2)*(n-3)*(3*n-8)/2.
 * https://oeis.org/A064197
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*(n-1)*(n-2)*(n-3)*(3*n-8)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence has offset 2: a(2)=0, a(3)=0. For n < 2 the formula
       would involve negative values which are invalid for unsigned types,
       so we return 0 for them. */
    if (n < 2)
        return 0;

    seq_t t = (seq_t)n;
    seq_t prod = t - 1;
    prod *= t - 2;
    prod *= t - 3;
    prod *= (3 * t - 8);
    prod *= 27;
    prod /= 2;
    return prod;
}
