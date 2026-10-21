/*
 * A218513  Number of n-colorings of the dodecahedral graph.
 * https://oeis.org/A218513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2) * * (n^17 - 27*n^16 + 352*n^15 - 2950*n^14
 *            + 17839*n^13 - 82777*n^12 + * 305866*n^11 - 921448*n^10 +
 *            2297495*n^9 - 4783425*n^8 + 8347700*n^7 - * 12195590*n^6 +
 *            14808795*n^5 - 14713381*n^4 + 11613602*n^3 - 6892084*n^2 + *
 *            2751604*n - 555984)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 10, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0), a(1), a(2) are zero because the factor n(n-1)(n-2) vanishes. */
    if (n < 3)
        return 0;

    /* Cast the index to the sequence type for all arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* Coefficients of the 17‑degree polynomial Q(n), ordered from
     * the term of degree 17 down to the constant term. */
    static const int64_t coeff[18] = {
        1LL,           /* n^17 */
        -27LL,         /* n^16 */
        352LL,         /* n^15 */
        -2950LL,       /* n^14 */
        17839LL,       /* n^13 */
        -82777LL,      /* n^12 */
        305866LL,      /* n^11 */
        -921448LL,     /* n^10 */
        2297495LL,     /* n^9  */
        -4783425LL,    /* n^8  */
        8347700LL,     /* n^7  */
        -12195590LL,   /* n^6  */
        14808795LL,    /* n^5  */
        -14713381LL,   /* n^4  */
        11613602LL,    /* n^3  */
        -6892084LL,    /* n^2  */
        2751604LL,     /* n^1  */
        -555984LL      /* n^0  */
    };

    /* Compute n^17 once: */
    seq_t pow = 1;
    for (int i = 0; i < 17; ++i)
        pow *= n_seq;          /* pow = n^17 */

    /* Accumulate positive and negative contributions separately.
     * Using unsigned arithmetic avoids intermediate underflows. */
    seq_t pos = 0;
    seq_t neg = 0;

    for (int i = 0; i < 18; ++i)
    {
        int64_t c = coeff[i];
        seq_t abscoef = (c >= 0) ? (seq_t)c : (seq_t)(-c);
        seq_t term = pow * abscoef;

        if (c >= 0)
            pos += term;
        else
            neg += term;

        /* Step to the next lower power of n, except after the last term. */
        if (i < 17)
            pow /= n_seq;
    }

    /* The polynomial value Q(n) = pos - neg (non‑negative for n >= 3). */
    seq_t poly = pos - neg;

    /* Final chromatic polynomial value. */
    return n_seq * (n_seq - 1) * (n_seq - 2) * poly;
}
