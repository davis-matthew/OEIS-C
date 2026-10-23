/*
 * A173724  Number of reduced, normalized 3 X 3 semimagic squares with
 *          distinct nonnegative integer entries and maximum entry n.
 * https://oeis.org/A173724
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3-33*n^2+174*n-264)/24 + (n mod 2)*(n^2-16*n+50)/8 *
 *            + ((2*n^2+1) mod 3)*(n-9)/3 + (n mod 3)*2/3 * - 2*[(n mod 4)=0]
 *            - 2*[(n mod 5)=0] (valid for n ≥ 8)
 * Range:     u8 n <= 20, u16 n <= 97, u32 n <= 3726, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    int64_t N   = (int64_t)n;
    int64_t N2  = N * N;
    int64_t N3  = N2 * N;

    /* part 1 numerator */
    int64_t num1 = 2 * N3 - 33 * N2 + 174 * N - 264;

    /* part 2 */
    int64_t odd  = n & 1;
    int64_t num2 = N2 - 16 * N + 50;
    int64_t term2 = odd ? 3 * num2 : 0;                 /* 3* ... to denom 24 */

    /* part 3 */
    int64_t t     = (2 * N2 + 1) % 3;
    int64_t term3 = 8 * t * (N - 9);                     /* 8* ... to denom 24 */

    /* part 4 */
    int64_t term4 = 16 * (n % 3);                       /* 16* ... to denom 24 */

    /* subtract the indicator terms */
    int64_t I4 = (n % 4 == 0) ? 1 : 0;
    int64_t I5 = (n % 5 == 0) ? 1 : 0;
    int64_t term5 = -48 * I4;   /* -48 * indicator to denominator 24 */
    int64_t term6 = -48 * I5;

    int64_t num_total = num1 + term2 + term3 + term4 + term5 + term6;
    seq_t result = (seq_t)(num_total / 24);
    return result;
}
