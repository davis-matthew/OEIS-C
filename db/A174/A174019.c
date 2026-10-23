/*
 * A174019  Number of symmetry classes of reduced 3 X 3 magilatin squares with
 *          largest entry n.
 * https://oeis.org/A174019
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3-9*n^2+54*n-96)/24 + (n mod 2)*(n^2-8*n+18)/8 +
 *            ((2*n^2+1) mod 3)*(n-3)/3 + (n mod 3)*2/3 - 2*([(n mod 4)=0]+[(n
 *            mod 5)=0])
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 2024, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for operations */
    seq_t N = (seq_t)n;

    /* Precompute powers */
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;

    /* Compute numerator with common denominator 24 */
    seq_t num = (seq_t)2 * N3 - (seq_t)9 * N2 + (seq_t)54 * N - (seq_t)96;

    /* (n mod 2)*(n^2-8*n+18)/8 -> multiply numerator by 3 to reach /24 */
    if (n & (idx_t)1)
        num += (seq_t)3 * (N2 - (seq_t)8 * N + (seq_t)18);

    /* ((2*n^2+1) mod 3)*(n-3)/3 -> multiply numerator by 8 to reach /24 */
    seq_t mod31 = ((seq_t)2 * N2 + (seq_t)1) % (seq_t)3;
    num += (seq_t)8 * mod31 * (N - (seq_t)3);

    /* (n mod 3)*2/3 -> multiply numerator by 8 (->16) to reach /24 */
    num += (seq_t)16 * (N % (seq_t)3);

    /* -2*([(n mod 4)=0]+[(n mod 5)=0]) -> subtract 48*add from numerator */
    unsigned add = 0;
    if ((n % 4) == 0) add++;
    if ((n % 5) == 0) add++;
    num -= (seq_t)48 * (seq_t)add;

    /* Final division by 24 gives the exact integer value */
    return num / (seq_t)24;
}
