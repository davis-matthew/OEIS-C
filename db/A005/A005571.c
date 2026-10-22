/*
 * A005571  Number of walks on cubic lattice.
 * https://oeis.org/A005571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 4(n+1)(n+3)(8n+19)/3
 * Range:     u8 n <= 0, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    seq_t a = n_seq + 1;
    seq_t b = n_seq + 3;
    seq_t c = 8 * n_seq + 19;
    
    /* One of {a, b, c} is divisible by 3; divide it early to avoid overflow */
    if (a % 3 == 0) {
        a /= 3;
    } else if (b % 3 == 0) {
        b /= 3;
    } else {
        c /= 3;
    }
    
    return 4 * a * b * c;
}
