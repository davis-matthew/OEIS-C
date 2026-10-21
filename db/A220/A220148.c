/*
 * A220148  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 n X 3 array.
 * https://oeis.org/A220148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the order‑12 linear recurrence a(n) = 12*a(n-1) -
 *            66*a(n-2) + 220*a(n-3) - 495*a(n-4) + 792*a(n-5) - 924*a(n-6) +
 *            792*a(n-7) - 495*a(n-8) + 220*a(n-9) - 66*a(n-10) + 12*a(n-11) -
 *            a(n-12) for n>12 with initial values
 *            a(1)=6,a(2)=23,a(3)=77,a(4)=242,a(5)=727,a(6)=2062,
 *            a(7)=5493,a(8)...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 32, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial terms (1‑based index) */
    static const seq_t init[12] = {
        (seq_t)6, (seq_t)23, (seq_t)77, (seq_t)242, (seq_t)727,
        (seq_t)2062, (seq_t)5493, (seq_t)13773, (seq_t)32664,
        (seq_t)73654, (seq_t)158660, (seq_t)327852
    };
    if (n <= 12) {
        return init[n-1];
    }

    /* Buffer of the last 12 computed values */
    seq_t last[12];
    for (idx_t i = 0; i < 12; ++i) last[i] = init[i];

    for (idx_t i = 13; i <= n; ++i) {
        seq_t next = 12*last[11]   - 66*last[10]
                   + 220*last[9]   - 495*last[8]
                   + 792*last[7]   - 924*last[6]
                   + 792*last[5]   - 495*last[4]
                   + 220*last[3]   - 66*last[2]
                   + 12*last[1]    - last[0];
        /* Shift left to make room for the new term */
        for (idx_t j = 0; j < 11; ++j) last[j] = last[j+1];
        last[11] = next;
    }
    return last[11];
}
