/*
 * A094584  Dot product of (1,2,...,n) and first n distinct Fibonacci numbers.
 * https://oeis.org/A094584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} k·F(k+1) (F(0)=0, F(1)=1)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 37, u64 n <= 81, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the dot product of (1,2,…,n) with the first n
   Fibonacci numbers.  The algorithm is O(n) per call and uses only
   local variables, so it is safe for any build (fixed‑width or
   arbitrary‑precision) and compiles in both C11 and C++17. */

seq_t a(idx_t n)
{
    seq_t fib0 = (seq_t)0;   /* F(0) */
    seq_t fib1 = (seq_t)1;   /* F(1) */
    seq_t sum  = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute F(i+1) = fib0 + fib1 */
        seq_t fib_next = fib0 + fib1;
        /* Add i * F(i+1) to the cumulative sum */
        sum += (seq_t)i * fib_next;
        /* Advance Fibonacci numbers for next iteration */
        fib0 = fib1;
        fib1 = fib_next;
    }
    return sum;
}
