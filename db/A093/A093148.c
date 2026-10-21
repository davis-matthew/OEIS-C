/*
 * A093148  a(n) = gcd(Fibonacci(n+5), Fibonacci(n+1)).
 * https://oeis.org/A093148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-4) for n>3; a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Periodic pattern of length 4: [1, 1, 1, 3] */
    uint64_t r = n % 4;          /* idx_t is unsigned (uint64_t) */
    switch (r) {
        case 0: return 1;
        case 1: return 1;
        case 2: return 1;
        default: return 3; /* r == 3 */
    }
}
