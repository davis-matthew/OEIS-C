/*
 * A188623  Number of reachable configurations in a chip-firing game on a
 *          triangle starting with n chips on one vertex.
 * https://oeis.org/A188623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3k^2 + 3k - 2)/2 if n%3==0, (3k^2 + 5k + 2)/2 if n%3==1,
 *            (3k^2 + 7k + 4)/2 if n%3==2, where k = n/3 (integer division)
 * Range:     u8 n <= 37, u16 n <= 625, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Obtain k = floor(n/3) and the remainder r = n mod 3 */
    seq_t k = (seq_t)(n / 3);
    idx_t r = n % 3;

    if (r == 0) {
        /* a(3k)   = (3k^2 + 3k - 2)/2 */
        return ((seq_t)3 * k * k + (seq_t)3 * k - (seq_t)2) / (seq_t)2;
    } else if (r == 1) {
        /* a(3k+1) = (3k^2 + 5k + 2)/2 */
        return ((seq_t)3 * k * k + (seq_t)5 * k + (seq_t)2) / (seq_t)2;
    } else { /* r == 2 */
        /* a(3k+2) = (3k^2 + 7k + 4)/2 */
        return ((seq_t)3 * k * k + (seq_t)7 * k + (seq_t)4) / (seq_t)2;
    }
}
