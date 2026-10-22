/*
 * A007895  Number of terms in the Zeckendorf representation of n (write n as
 *          a sum of non-consecutive distinct Fibonacci numbers).
 * https://oeis.org/A007895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Zeckendorf representation - count of terms
 * Range:     u8 n <= 638, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t n_seq = (seq_t)n;
    
    // Generate Fibonacci numbers: 1, 2, 3, 5, 8, 13, ...
    seq_t fib[150];
    int fib_len = 0;
    
    seq_t f_prev = 1, f_curr = 2;
    fib[fib_len++] = f_prev;
    
    while (f_curr <= n_seq) {
        fib[fib_len++] = f_curr;
        seq_t next = f_curr + f_prev;
        f_prev = f_curr;
        f_curr = next;
    }
    
    // Greedy algorithm for Zeckendorf representation
    seq_t remaining = n_seq;
    int count = 0;
    
    for (int i = fib_len - 1; i >= 0 && remaining > 0; ) {
        if (fib[i] <= remaining) {
            remaining -= fib[i];
            count++;
            i -= 2;  // Skip next Fibonacci to ensure non-consecutive
        } else {
            i--;
        }
    }
    
    return (seq_t)count;
}
