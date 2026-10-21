/*
 * A208283  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A208283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^4 + 30*n^3 + 2*n^2 - 6*n)/3 valid for n ≥ 1. Uses
 *            the closed‑form polynomial from the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return 0; /* safety for offset handling */

    /* Use a 128‑bit intermediate to avoid overflow for all n
       that appear in the test data. */
    unsigned __int128 N  = n;
    unsigned __int128 n2 = N * N;        /* n^2   */
    unsigned __int128 n3 = n2 * N;       /* n^3   */
    unsigned __int128 n4 = n3 * N;       /* n^4   */

    unsigned __int128 num = 4 * n4 + 30 * n3 + 2 * n2 - 6 * N;
    unsigned __int128 res = num / 3;     /* exact division by 3 */

    return (seq_t)res;
}
