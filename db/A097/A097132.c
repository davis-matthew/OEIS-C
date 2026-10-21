/*
 * A097132  a(n) = Sum_{k=0..n} Fibonacci(k) + (-1)^k*Fibonacci(k-1).
 * https://oeis.org/A097132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) - a(n-4) + a(n-5)
 * Initial values: a(0)=1  a(1)=2  a(2)=4  a(3)=5  a(4)=10
 * This recurrence follows from the definition
 * a(n) = Σ_{k=0..n} [F(k) + (-1)^k F(k-1)].
 */
seq_t a(idx_t n)
{
    /* Pre‑computed first five terms (indices 0..4) */
    static const seq_t init[5] = {1, 2, 4, 5, 10};

    if (n < 5)
        return init[n];

    /* State array holds the last five computed values.
       state[i % 5] will contain a(i). */
    seq_t state[5];
    for (int i = 0; i < 5; ++i)
        state[i] = init[i];

    /* Compute up to index n */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t t1 = state[(i - 1) % 5];
        seq_t t2 = state[(i - 2) % 5];
        seq_t t3 = state[(i - 3) % 5];
        seq_t t4 = state[(i - 4) % 5];
        seq_t t5 = state[(i - 5) % 5];

        /* Positive contributions */
        seq_t pos = t1 + t2 * 3 + t5;
        /* Negative contributions */
        seq_t neg = t3 * 3 + t4;

        /* Derived term is guaranteed non‑negative and fits in seq_t. */
        seq_t next = pos - neg;

        state[i % 5] = next;
    }

    return state[n % 5];
}
