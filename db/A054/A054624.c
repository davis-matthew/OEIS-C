/*
 * A054624  Number of ways to color vertices of a 10-gon using <= n colors,
 *          allowing only rotations.
 * https://oeis.org/A054624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^8-n^7+n^6-n^5+n^4+4)/10
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 11, u64 n <= 106, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a0 = (seq_t)n;                /* n */
    seq_t b0 = a0 + (seq_t)1;           /* n+1 */

    /* n^2 … n^8 */
    seq_t p2 = a0 * a0;                 /* n^2   */
    seq_t p3 = p2 * a0;                 /* n^3   */
    seq_t p4 = p3 * a0;                 /* n^4   */
    seq_t p5 = p4 * a0;                 /* n^5   */
    seq_t p6 = p5 * a0;                 /* n^6   */
    seq_t p7 = p6 * a0;                 /* n^7   */
    seq_t p8 = p7 * a0;                 /* n^8   */

    /* c = n^8-n^7+n^6-n^5+n^4+4 */
    seq_t c0 = p8 - p7 + p6 - p5 + p4 + (seq_t)4;

    /* cancel the factor 10 (=2·5) across the three factors */
    seq_t f[3] = {a0, b0, c0};
    unsigned rem = 10;                  /* remaining divisor */

    for (int i = 0; i < 3 && rem > 1; ++i) {
        while (rem % 2 == 0 && (f[i] % (seq_t)2) == 0) {
            f[i] /= (seq_t)2;
            rem /= 2;
        }
        while (rem % 5 == 0 && (f[i] % (seq_t)5) == 0) {
            f[i] /= (seq_t)5;
            rem /= 5;
        }
    }

    /* product of the reduced factors; rem should be 1 */
    seq_t result = f[0];
    result *= f[1];
    result *= f[2];

    return result;
}
