/*
 * A000433  n written in base where place values are positive cubes.
 * https://oeis.org/A000433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   greedy representation in base with place values 1^3, 2^3, 3^3,
 * Range:     u8 n <= 63, u16 n <= 215, u32 n <= 1330, u64 n <= 9260,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t remainder = (seq_t)n;
    
    // Find the maximum k such that k^3 <= remainder
    idx_t max_k = 1;
    while (max_k < 1000000) {
        seq_t next_k_seq = (seq_t)(max_k + 1);
        seq_t next_k_cubed = next_k_seq * next_k_seq * next_k_seq;
        if (next_k_cubed > remainder) break;
        max_k++;
    }
    
    // Build the result by concatenating digits from largest k down to 1
    seq_t result = 0;
    for (idx_t k = max_k; k >= 1; k--) {
        seq_t k_seq = (seq_t)k;
        seq_t k_cubed = k_seq * k_seq * k_seq;
        seq_t digit = remainder / k_cubed;
        remainder = remainder % k_cubed;
        result = result * 10 + digit;
    }
    
    return result;
}
