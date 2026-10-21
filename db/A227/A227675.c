/*
 * A227675  Number of n X 2 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 3 binary array having an odd sum, with rows and
 *          columns of the latter in lexicographically nondecreasing order.
 * https://oeis.org/A227675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 15*n^5 + 145*n^4 + 225*n^3 + 1294*n^2 + 480*n +
 *            720)/720
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 118, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1)
        return 0;

    /* Use 128‑bit arithmetic for the intermediate polynomial evaluations.
       The final division produces the exact integer result. */
    unsigned __int128 x = (unsigned __int128)n;
    unsigned __int128 p = 1;
    p = p * x + 15;
    p = p * x + 145;
    p = p * x + 225;
    p = p * x + 1294;
    p = p * x + 480;
    p = p * x + 720;          /* p now holds the numerator n^6 + … + 720 */

    return (seq_t)(p / 720);
}
