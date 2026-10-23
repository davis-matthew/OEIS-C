/*
 * A184655  floor(n*s+h-h*s), where s=(3+sqrt(3))/2, h=-2/3; complement of
 *          A184654.
 * https://oeis.org/A184655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ((9*n+2) + (3*n+2)*sqrt(3) ) / 6 )
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A184655: a(n) = floor(n*(3+sqrt(3))/2 + (1+sqrt(3))/3) */

seq_t a(idx_t n)
{
    /* rational approximation of sqrt(3) with denominator 10^15  */
    const unsigned long long DEN     = 1000000000000000ULL;          /* 10^15          */
    const unsigned long long NUM_LOW = 1732050807568877ULL;          /* floor(sqrt(3)*DEN)   */
    const unsigned long long NUM_HIGH = NUM_LOW + 1ULL;             /* ceil(sqrt(3)*DEN)    */
    const unsigned long long DEN6   = 6ULL * DEN;                    /* 6*DEN           */

    /* convert n (uint64_t) to a native unsigned long long for calculations */
    const unsigned long long n_ll = (unsigned long long)n;

    /* intermediate integers: 9*n + 2  and  3*n + 2  */
    const unsigned long long mA = 9ULL * n_ll + 2ULL;   /* 9*n + 2 */
    const unsigned long long mB = 3ULL * n_ll + 2ULL;   /* 3*n + 2 */

    /* compute all parts in 128‑bit to avoid overflow */
    unsigned __int128 partA  = (unsigned __int128)mA * DEN;           /* (9*n+2)*DEN */
    unsigned __int128 partLow = (unsigned __int128)mB * NUM_LOW;     /* (3*n+2)*NUM_LOW */
    unsigned __int128 partHigh = (unsigned __int128)mB * NUM_HIGH;   /* (3*n+2)*NUM_HIGH */

    unsigned __int128 sumLow  = partA + partLow;
    unsigned __int128 sumHigh = partA + partHigh;

    /* floor using lower and upper bounds */
    seq_t low_val  = (seq_t)(sumLow  / DEN6);
    seq_t high_val = (seq_t)(sumHigh / DEN6);

    /* if bounds coincide, the value is exact, otherwise it must have used the
       higher bound (at most one more). */
    return (low_val == high_val) ? low_val : high_val;
}
