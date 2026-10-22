/*
 * A004780  Binary expansion contains 2 adjacent 1's.
 * https://oeis.org/A004780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n-th positive integer whose binary expansion contains two adjacent 1's */
seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    seq_t target = (seq_t)n;
    
    while (count < target) {
        num++;
        if ((num & (num << 1)) != 0) {
            count++;
        }
    }
    
    return num;
}
