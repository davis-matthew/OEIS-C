/*
 * A193397  Wiener index of a benzenoid consisting of a double-step spiral
 *          chain of n hexagons (n>=2, s=21; see the Gutman et al. reference).
 * https://oeis.org/A193397
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + 20*n^2 - 12*n + 2*(-1)^n*(n-2) + 21 (for n >= 2,
 *            the sequence has offset 2)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Return 0 for indices below the offset; behavior undefined for n < 2. */
    if (n < 2) {
        return 0;
    }

    /* Use 64‑bit signed arithmetic for intermediate calculations.
       For the range of n relevant to the tests (up to about 10 000),
       all intermediate values fit in a signed long long without overflow. */
    long long nn = (long long)n;
    long long n2 = nn * nn;
    long long n3 = n2 * nn;

    long long sum = 4 * n3 + 20 * n2 - 12 * nn + 21;

    if ((nn & 1) == 0) {          /* even n: (-1)^n = 1 */
        sum += 2 * (nn - 2);
    } else {                       /* odd n:  (-1)^n = -1 */
        sum -= 2 * (nn - 2);
    }

    return (seq_t)sum;
}
