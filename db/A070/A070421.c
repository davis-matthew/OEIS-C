/*
 * A070421  a(n) = 7^n mod 38.
 * https://oeis.org/A070421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 38
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Period 3: [1, 7, 11] */
    switch ((unsigned)(n % 3)) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)7;
        case 2: return (seq_t)11;
    }
    return (seq_t)0; /* unreachable */
}
