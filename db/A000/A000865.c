/*
 * A000865  Numbers beginning with letter 'o' in English.
 * https://oeis.org/A000865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   OEIS A000865 - Numbers starting with 'o' (one) in English: 1,
 *            100-199, 1000-1999, 100000-199999,
 * Range:     u8 n <= 101, u16 n <= 1101, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    
    idx_t remaining = n - 2;
    
    for (int block = 0; ; block++) {
        int power = (block / 2) * 3 + 2 + (block % 2);
        
        seq_t block_size = 1;
        for (int i = 0; i < power; i++) {
            block_size *= 10;
        }
        
        if (remaining < (idx_t)block_size) {
            return block_size + (seq_t)remaining;
        }
        
        remaining -= (idx_t)block_size;
    }
}
