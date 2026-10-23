/*
 * A181532  a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 2; a(n) = a(n-1) + a(n-2) +
 *          a(n-4).
 * https://oeis.org/A181532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4); a(0)=0, a(1)=1, a(2)=1, a(3)=2;
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A181532 using a simple O(n) iterative recurrence. */

seq_t a(idx_t n) {
    /* Handle the initial four values */
    static const seq_t init[4] = {(seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2};
    if (n < 4) {
        return init[n];
    }

    /* Use a circular buffer of size 4 to hold the last four terms. */
    seq_t buf[4] = {init[0], init[1], init[2], init[3]};

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = buf[(i - 1) & 3] + buf[(i - 2) & 3] + buf[(i - 4) & 3];
        buf[i & 3] = next;
    }

    return buf[n & 3];
}
