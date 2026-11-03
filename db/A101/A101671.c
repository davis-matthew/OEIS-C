/*
 * A101671  Fixed point of morphism 0 -> 01, 1 -> 20, 2 -> 10.
 * https://oeis.org/A101671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 For n > 0 let p = a(n/2). If n is even, a(n) =
 *            first_child(p) If n is odd, a(n) = second_child(p) where:
 *            first_child(0) = 0, first_child(1) = 2, first_child(2) = 1
 *            second_child(0) = 1, second_child(1) = 0, second_child(2) = 0
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: partial, pass, 102 terms through n = 101 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t first_child(seq_t x) {
    switch ((int)x) {
        case 0: return 0;
        case 1: return 2;
        case 2: return 1;
        default: return 0;   /* unreachable */
    }
}

static inline seq_t second_child(seq_t x) {
    switch ((int)x) {
        case 0: return 1;
        case 1: return 0;
        case 2: return 0;
        default: return 0;   /* unreachable */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* Recurse on the parent symbol at position n/2 */
    seq_t parent = a(n >> 1);
    return ((n & 1) == 0) ? first_child(parent) : second_child(parent);
}
