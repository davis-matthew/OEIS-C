/*
 * A047505  Numbers that are congruent to {0, 1, 2, 3, 6, 7} mod 8.
 * https://oeis.org/A047505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*⌊(n-1)/6⌋ + {0,1,2,3,6,7}[ (n-1) mod 6 ]
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0-based offset: a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=6, a(6)=7, ...   */
seq_t a(idx_t n) {
    /* If the caller passes n == 0 (outside the OEIS offset), return 0. */
    if (n == 0) {
        return 0;
    }

    static const seq_t rem[6] = {0, 1, 2, 3, 6, 7};

    idx_t m = n - 1;          /* non‑negative */
    idx_t k = m / 6;          /* number of complete 6‑term blocks before n */
    idx_t r = m % 6;          /* position within the current block (0..5) */

    /* Compute the value as per the formula. */
    seq_t val = (seq_t)8 * (seq_t)k + rem[r];
    return val;
}
