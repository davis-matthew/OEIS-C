/*
 * A219527  a(n) = (6*n^2 + 7*n - 9 + 2*n^3)/12 - (-1)^n*(n+1)/4.
 * https://oeis.org/A219527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 6*n^2 + 7*n - 9 - 3*(-1)^n*(n+1))/12
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where n >= 1 (offset 1) */

/* The implementation is fully integer arithmetic. */
/* For even n we subtract 3*(n+1), for odd n we add 3*(n+1). */
seq_t a(idx_t n)
{
    /* Promote n to seq_t once for all arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute the cubic and quadratic terms. */
    seq_t num = 2*nn*nn*nn      /* 2*n^3      */
              + 6*nn*nn        /* 6*n^2      */
              + 7*nn           /* +7*n       */
              - 9;             /* -9         */

    /* Adjust for the alternating sign part */
    if ((n & 1ULL) == 0ULL) /* even n */
        num -= 3*(nn + 1);  /* subtract 3*(n+1) */
    else                   /* odd n  */
        num += 3*(nn + 1);  /* add 3*(n+1)      */

    /* Finally divide by 12 – the value is guaranteed to be an integer. */
    return num / 12;
}
