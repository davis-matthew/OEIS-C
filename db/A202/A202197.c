/*
 * A202197  Number of (n+2) X 5 binary arrays avoiding patterns 001 and 101 in
 *          rows and columns.
 * https://oeis.org/A202197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+5)*(n+4)*(n+3)*(n+2)^2/12
 * Range:     u8 none, u16 n <= 7, u32 n <= 97, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* Factors of the formula: (n+2) twice, then (n+3),(n+4),(n+5), and 5. */
    seq_t f[6];
    f[0] = nseq + 2;
    f[1] = f[0];
    f[2] = nseq + 3;
    f[3] = nseq + 4;
    f[4] = nseq + 5;
    f[5] = 5;

    /* Cancel the denominator 12 = 2^2 * 3 as early as possible to avoid overflow. */
    int need2 = 2;   /* two factors of 2 */
    int need3 = 1;   /* one factor of 3 */

    for (int i = 0; i < 6; ++i) {
        /* Remove as many 2's as possible from this factor. */
        while (need2 && (f[i] & 1) == 0) {
            f[i] >>= 1;   /* divide by 2 */
            need2--;
        }
        /* Remove as many 3's as possible from this factor. */
        while (need3 && (f[i] % 3) == 0) {
            f[i] /= 3;    /* divide by 3 */
            need3--;
        }
    }

    /* Multiply all reduced factors together.  The intermediate results never
       exceed the final answer, which is guaranteed to fit in seq_t (for the
       variable‑size build, or for the 64‑bit build for numbers that fit). */
    seq_t result = 1;
    for (int i = 0; i < 6; ++i) {
        result *= f[i];
    }
    return result;
}
