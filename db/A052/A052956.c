/*
 * A052956  a(n) = 2^n + Fibonacci(n+1).
 * https://oeis.org/A052956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052956: a(n) = 2^n + Fibonacci(n+1)
 * offset = 0   (a(0) is the first listed term)
 * recurrence (from the OEIS entry): a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3)  for n ≥ 3
 * This implementation computes the term directly from the definition.
 */

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    /* compute Fibonacci(n+1) iteratively */
    seq_t f0 = 0, f1 = 1, fn = 1;    /* f1 currently fib(1) */
    if (n > 0) {
        for (idx_t i = 0; i < n; ++i) {
            seq_t next = f0 + f1;
            f0 = f1;
            f1 = next;
        }
        fn = f1;   /* now f1 == fib(n+1) */
    }

    /* sum the two components */
    return pow2 + fn;
}
