/*
 * A004209  For m=2,3,..., write m in bases m,m-1,...,3,2.
 * https://oeis.org/A004209
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For m >= 2, write m in bases m, m-1, ..., 3, 2; read digits as decimal. */

seq_t a(idx_t n) {
    // Find m such that (m-1)*m/2 >= n - 1
    idx_t m = 2;
    while ((m - 1) * m / 2 < n - 1) {
        m++;
    }
    
    // Position within m's group (1-indexed)
    idx_t lower = (m - 2) * (m - 1) / 2;
    idx_t pos = (n - 1) - lower;
    
    // Base to use (from m down to 2)
    idx_t base = m + 1 - pos;
    
    // Convert m to the given base and read digits as decimal
    seq_t result = 0;
    seq_t m_val = (seq_t)m;
    seq_t power = 1;
    
    while (m_val > 0) {
        seq_t digit = m_val % (seq_t)base;
        result += digit * power;
        power *= 10;
        m_val /= (seq_t)base;
    }
    
    return result;
}
