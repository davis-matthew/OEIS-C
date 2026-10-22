/*
 * A005356  Number of low discrepancy sequences in base 2.
 * https://oeis.org/A005356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   block-based structure where block b contains values incrementing
 *            by b, with block sizes following: b[0]=2, b[1]=1, b[2]=2,
 *            b[i]=b[i-2]+b[i-1] for odd i≥3, b[i]=2·b[i-1] for even i≥4
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return 0;
    
    // Compute block sizes
    idx_t block_sizes[256];
    block_sizes[0] = 2;
    block_sizes[1] = 1;
    block_sizes[2] = 2;
    
    for (int i = 3; i < 256; i++) {
        if (i % 2 == 1) {
            block_sizes[i] = block_sizes[i-2] + block_sizes[i-1];
        } else {
            block_sizes[i] = 2 * block_sizes[i-1];
        }
    }
    
    // Find which block contains position n
    idx_t cumsum = 0;
    int block_num = 0;
    for (int b = 0; b < 256; b++) {
        if (cumsum + block_sizes[b] >= n) {
            block_num = b;
            break;
        }
        cumsum += block_sizes[b];
    }
    
    // Compute the last value at the end of each block up to block_num-1
    seq_t last_val = 0;
    for (int b = 1; b < block_num; b++) {
        seq_t first_val = last_val + (seq_t)b;
        last_val = first_val + (seq_t)(block_sizes[b] - 1) * (seq_t)b;
    }
    
    // Compute the value at position n within block block_num
    seq_t first_val_in_block = last_val + (seq_t)block_num;
    idx_t pos_in_block = n - cumsum;
    seq_t result = first_val_in_block + (seq_t)(pos_in_block - 1) * (seq_t)block_num;
    
    return result;
}
