/*
 * A052406  Numbers without 4 as a digit.
 * https://oeis.org/A052406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = replace digits d > 3 by d+1 in base-9 representation of
 *            n-1
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base-9 conversion of (n-1) with digit mapping. */
    if (n == 0) /* not used by the sequence, but harmless */
        return 0;

    idx_t k = n - 1;      /* number to convert */
    seq_t result = 0;     /* accumulated decimal value */
    seq_t power  = 1;     /* 10^position in the result */

    while (k != 0) {
        unsigned d = k % 9;                 /* base-9 digit (0..8) */
        unsigned m = (d < 4) ? d : d + 1;   /* map 4-8 to 5-9 */
        result += (seq_t)m * power;         /* add digit in place */
        power  *= 10;                       /* advance decimal position */
        k      /= 9;
    }

    return result;
}
