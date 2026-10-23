/*
 * A130237  The 'lower' Fibonacci Inverse A130233(n) multiplied by n.
 * https://oeis.org/A130237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3640, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = n * k   where k = max{m ≥ 0 : Fib(m) ≤ n}
 *           (Fib(0)=0, Fib(1)=1, Fib(2)=1, Fib(3)=2, …)
 * This finds the largest Fibonacci index whose value does not exceed n,
 * then returns the product n*k.
 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Use seq_t for Fibonacci values to avoid overflow of 64‑bit indices. */
    seq_t f0 = 0;            /* Fib(i)   */
    seq_t f1 = 1;            /* Fib(i+1) */
    idx_t idx = 0;           /* current i */
    idx_t k   = 0;           /* largest i with Fib(i) ≤ n */

    while (f0 <= (seq_t)n) {
        k = idx;            /* current Fib(i) ≤ n */
        ++idx;              /* advance to next index */

        /* compute Fib(i+2) = Fib(i) + Fib(i+1) */
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }

    return (seq_t)n * (seq_t)k;
}
