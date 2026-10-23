/*
 * A174020  Number of reduced 3 X 3 magilatin squares with magic sum n.
 * https://oeis.org/A174020
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^3-45*n^2+340*n-456)/2 + (n mod
 *            2)*(9*n^2-135*n+300)/2 + (((n+2) mod 3) - (n mod 3))*8*n +
 *            (((n+3) mod 4) + ((n+2) mod 4) - ((n+1) mod 4) - (n mod
 *            4))*9*n/4 - 144*[(n mod 3)=0] - 180*[(n mod 4)=0] - 108*[(n mod
 *            5)=0] - 144*[(n mod 6)=0] - 72*[(n mod 7)=0] - 72*[(n mod 8)=0]
 *            where [P] i...
 * Range:     u8 n <= 8, u16 n <= 38, u32 n <= 1424, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset (n < 3). */
    if (n < 3)
        return 0;

    /* Use signed intermediate to handle subtractions safely. */
    int64_t N = (int64_t)n;
    int64_t N2 = N * N;
    int64_t N3 = N2 * N;

    int64_t term1 = (3 * N3 - 45 * N2 + 340 * N - 456) / 2;

    int64_t term2 = 0;
    if (n & 1)                /* n mod 2 == 1 for odd n */
        term2 = (9 * N2 - 135 * N + 300) / 2;

    int64_t diff3 = (int64_t)((n + 2) % 3) - (int64_t)(n % 3);
    int64_t term3 = diff3 * 8 * N;

    int64_t coef4 = (int64_t)((n + 3) % 4)
                  + (int64_t)((n + 2) % 4)
                  - (int64_t)((n + 1) % 4)
                  - (int64_t)(n % 4);
    int64_t term4 = (coef4 * 9 * N) / 4;

    int64_t res = term1 + term2 + term3 + term4;

    if (n % 3 == 0) res -= 144;
    if (n % 4 == 0) res -= 180;
    if (n % 5 == 0) res -= 108;
    if (n % 6 == 0) res -= 144;
    if (n % 7 == 0) res -= 72;
    if (n % 8 == 0) res -= 72;

    return (seq_t)res;
}
