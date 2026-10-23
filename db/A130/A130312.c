/*
 * A130312  Each Fibonacci number F(n) appears F(n) times.
 * https://oeis.org/A130312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = A000045(A072649(n)) Each Fibonacci number F(k) appears
 *            exactly F(k) times. a(n) is the first F(k) such that the
 *            cumulative sum of F(1) to F(k) is at least n
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the Fibonacci function fib(idx_t) defined elsewhere.
   Uses only integer operations allowed on seq_t and idx_t. */

seq_t a(idx_t n) {
    /* Treat n as a seq_t for comparison with accumulations. */
    seq_t target = (seq_t)n;

    /* Initial Fibonacci terms: F(1) = 1, F(2) = 1 */
    seq_t prev = 0;   /* F(0) */
    seq_t curr = 1;   /* F(1) */
    seq_t cum  = 0;   /* cumulative sum */

    for (;;) {
        /* Add current Fibonacci number to cumulative sum. */
        cum += curr;
        /* If cumulative sum reaches or exceeds n, we have found the term. */
        if (cum >= target)
            return curr;

        /* Advance to next Fibonacci number: F(k+1) = F(k) + F(k-1). */
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
}
