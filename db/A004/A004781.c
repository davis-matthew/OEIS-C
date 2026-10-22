/*
 * A004781  Binary expansion contains 3 adjacent 1's.
 * https://oeis.org/A004781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers whose binary expansion contains 3 adjacent 1's
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (count < n) {
        num++;
        if ((num & (num >> 1) & (num >> 2)) != 0) {
            count++;
        }
    }
    
    return num;
}
