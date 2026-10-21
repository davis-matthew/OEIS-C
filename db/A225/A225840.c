/*
 * A225840  Largest number less than n occurring in Collatz trajectory
 *          starting with n.
 * https://oeis.org/A225840
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 2494, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by simulating the Collatz trajectory from n
 * and tracking the largest number less than n that occurs in that trajectory. */
seq_t a(idx_t n) {
    if (n <= 1) return 0; /* undefined for n < 2, but harmless */
    seq_t N = (seq_t)n;
    seq_t cur = N;
    seq_t max_less = 0;

    /* Traverse the Collatz sequence until reaching 1 */
    while (cur != (seq_t)1) {
        if (cur < N && cur > max_less) {
            max_less = cur;
        }
        /* next term */
        if ((cur & (seq_t)1) == 0) {
            cur >>= 1;          /* divide by 2 if even */
        } else {
            cur = cur * (seq_t)3 + (seq_t)1; /* 3x + 1 if odd */
        }
    }
    /* Process the final value 1 */
    if ((seq_t)1 < N && (seq_t)1 > max_less) {
        max_less = (seq_t)1;
    }
    return max_less;
}
