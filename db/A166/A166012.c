/*
 * A166012  a(n) = 2*(A000045(n)-(n mod 2)) + 1 + (n mod 2).
 * https://oeis.org/A166012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fib(n) + (n mod 2 == 0 ? 1 : 0)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Fibonacci(n) using an O(n) iterative algorithm.  The
   sequence is indexed starting with Fib(0)=0, Fib(1)=1. */
static seq_t fib_idx(idx_t n)
{
    seq_t a = 0;      /* Fib(0)  */
    seq_t b = 1;      /* Fib(1)  */

    for (idx_t i = 0; i < n; ++i) {
        seq_t c = a + b; /* next Fibonacci number */
        a = b;
        b = c;
    }
    return a; /* a = Fib(n) after the loop */
}

seq_t a(idx_t n)
{
    seq_t fn   = fib_idx(n);          /* Fib(n) */
    seq_t twice = fn * (seq_t)2;      /* 2*Fib(n) */

    /* Add 1 only for even n */
    if ((n & (idx_t)1) == 0)
        return twice + (seq_t)1;
    else
        return twice;
}
