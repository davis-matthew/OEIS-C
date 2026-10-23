/*
 * A173729  Number of symmetry classes of 3 X 3 magilatin squares with
 *          positive values < n.
 * https://oeis.org/A173729
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5/240 - 3*n^4/64 + 97*n^3/216 - 2029*n^2/720 + 17*n/2 -
 *            41/5 + (n mod 2)*(2*n^2-20*n+101)/64 - ((2*n^2+2*n) mod
 *            3)*(9*n-75)/81 - ((2*n^2+n+2) mod 5)*2/5 + [(n mod 3)=2]*8/27 -
 *            [(n mod 4)=3]
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 254, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const __int128 D = 25920;            /* common denominator */
    __int128 n128 = (__int128)n;
    __int128 n2   = n128 * n128;
    __int128 n3   = n2   * n128;
    __int128 n4   = n3   * n128;
    __int128 n5   = n4   * n128;

    __int128 N = 0;

    /* polynomial part with denominators absorbed into coefficients */
    N += n5   * 108;           // n^5 * 108   (108 = 25920/240)
    N -= n4   * 1215;          // -3*n^4 * 405 (405 = 25920/64) -> -1215
    N += n3   * 11640;         // +97*n^3 * 120 (120 = 25920/216)
    N -= n2   * 73044;         // -2029*n^2 * 36 (36 = 25920/720)
    N += n128 * 220320;        // +17*n * 12960 (12960 = 25920/2)
    N -= 212544;               // -41 * (25920/5)

    /* periodic terms */
    if (n & 1) {                                  // (n mod 2)
        __int128 inside = 2 * n2 - 20 * n128 + 101;
        N += inside * 405;                         // (25920/64) = 405
    }

    /* ((2*n^2+2*n) mod 3) * (9*n-75) / 81  */
    int mod3 = (int)((2 * n2 + 2 * n128) % 3);
    if (mod3) {
        __int128 inside8 = 9 * n128 - 75;
        N -= (__int128)mod3 * inside8 * 320;      // 25920/81 = 320
    }

    /* ((2*n^2+n+2) mod 5) * 2 / 5  */
    int mod5 = (int)((2 * n2 + n128 + 2) % 5);
    if (mod5) {
        N -= (__int128)mod5 * 10368;              // 2 * 5184 = 10368, 5184=25920/5
    }

    /* [(n mod 3)=2]*8/27 */
    if ((n % 3) == 2) {
        N += 7680;                                 // 8 * 960, 960=25920/27
    }

    /* -[(n mod 4)=3]  */
    if ((n % 4) == 3) {
        N -= D;                                     // subtract D for the integer term
    }

    /* Final division by common denominator */
    seq_t result = (seq_t)(N / D);
    return result;
}
