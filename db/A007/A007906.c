/*
 * A007906  Number of steps for aliquot sequence for n to converge to 0, or -1
 *          if it never reaches 0.
 * https://oeis.org/A007906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   aliquot sequence steps to 0; sum of proper divisors iteratively
 * Range:     i8 n <= 29, i16 n <= 275, i32 n <= 275, i64 n <= 275,
 *            i128 n <= 275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t current = (seq_t)n;
    seq_t steps = 0;
    
    // Track seen values for cycle detection
    seq_t seen[1000];
    int seen_count = 0;
    
    while (current != 0) {
        // Check for cycle
        for (int i = 0; i < seen_count; i++) {
            if (seen[i] == current) {
                return -1;
            }
        }
        
        // Avoid buffer overflow
        if (seen_count >= 1000) {
            return -1;
        }
        
        // Record current value
        seen[seen_count++] = current;
        
        // Compute sum of proper divisors
        seq_t sum_divisors = 0;
        for (seq_t i = 1; i * i <= current; i++) {
            if (current % i == 0) {
                if (i < current) {
                    sum_divisors += i;
                }
                seq_t other = current / i;
                if (other != i && other < current) {
                    sum_divisors += other;
                }
            }
        }
        
        current = sum_divisors;
        steps++;
    }
    
    return steps;
}
