/*
 * A140236  a(n) = A000292(A000292(n)).
 * https://oeis.org/A140236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1)*(n+2)*(n+3)*(2 + n^2)*(12 + n*(n+1)*(n+2)))/1296
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 25, u64 n <= 305,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial zero case first */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Construct the six multiplicative terms of the numerator */
    seq_t nn = (seq_t)n;          /* n                     */
    seq_t t1 = nn;                /* n                     */
    seq_t t2 = nn + 1;            /* n + 1                 */
    seq_t t3 = nn + 2;            /* n + 2                 */
    seq_t t4 = nn + 3;            /* n + 3                 */
    seq_t t5 = nn * nn + 2;       /* 2 + n^2               */
    seq_t t6 = 12 + nn * t2 * t3; /* 12 + n(n+1)(n+2)      */

    /* Keep track of the 2^4 and 3^4 factors that must be cancelled
       from the product to avoid overflow. */
    unsigned rem2 = 4; /* number of 2's still to cancel */
    unsigned rem3 = 4; /* number of 3's still to cancel */

    seq_t terms[6] = { t1, t2, t3, t4, t5, t6 };
    seq_t result = 1;

    for (int i = 0; i < 6; ++i) {
        seq_t term = terms[i];

        /* Cancel powers of 2 from the current term */
        while (rem2 > 0 && (term & 1) == 0) {
            term >>= 1;
            --rem2;
        }
        /* Cancel powers of 3 from the current term */
        while (rem3 > 0 && term % 3 == 0) {
            term /= 3;
            --rem3;
        }

        /* Multiply into the running product */
        result *= term;

        /* If the product now contains any of the remaining 2's or 3's,
           cancel them immediately so that the product stays small. */
        while (rem2 > 0 && (result & 1) == 0) {
            result >>= 1;
            --rem2;
        }
        while (rem3 > 0 && result % 3 == 0) {
            result /= 3;
            --rem3;
        }
    }

    /* Any remaining factors of 2 or 3 that were not cancelled earlier
       are divided out now. The division is exact by the definition of
       the sequence. */
    while (rem2 > 0) {
        result >>= 1;
        --rem2;
    }
    while (rem3 > 0) {
        result /= 3;
        --rem3;
    }

    return result;
}
