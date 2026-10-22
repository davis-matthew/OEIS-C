/*
 * A001266  One-half the number of permutations of length n without rising or
 *          falling successions.
 * https://oeis.org/A001266
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) - (n-2)*a(n-2) - (n-5)*a(n-3) + (n-3)*a(n-4)
 *            for n >= 6, a(2) = 0, a(3) = 0, a(4) = 1, a(5) = 7 (half of
 *            A002464, which satisfies the same recurrence)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t b2, b3, b4, b5, c;
    idx_t k;
    if (n < 2) return 0;
    if (n == 2 || n == 3) return 0;
    if (n == 4) return 1;
    if (n == 5) return 7;
    /* b2 = a(k-4), b3 = a(k-3), b4 = a(k-2), b5 = a(k-1) */
    b2 = 0; b3 = 0; b4 = 1; b5 = 7;
    for (k = 6; k <= n; k++) {
        seq_t pos = (seq_t)(k + 1) * b5 + (seq_t)(k - 3) * b2;
        seq_t neg = (seq_t)(k - 2) * b4 + (seq_t)(k - 5) * b3;
        c = pos - neg;
        b2 = b3;
        b3 = b4;
        b4 = b5;
        b5 = c;
    }
    return b5;
}
