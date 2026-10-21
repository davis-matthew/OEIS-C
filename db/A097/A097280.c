/*
 * A097280  Perimeter of integer triangle (A001611(n), A001611(n+1),
 *          A001611(n+2)).
 * https://oeis.org/A097280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(n+2) + 3 Fibonacci is computed by the
 *            fast‑doubling method. Fibonacci(n) is returned in *fk,
 *            Fibonacci(n+1) in *fk1
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t k, seq_t *fk, seq_t *fk1)
{
    if (k == (idx_t)0)
    {
        *fk  = (seq_t)0;
        *fk1 = (seq_t)1;
        return;
    }
    seq_t f_k, f_k1;
    fib_pair(k >> 1, &f_k, &f_k1);           /* f_k = F(m), f_k1 = F(m+1), m = k/2 */

    /* F(2m) = F(m) * (2*F(m+1) - F(m)) */
    seq_t two_f_k1 = f_k1 + f_k1;
    seq_t two_f_k1_minus_f_k = two_f_k1 - f_k;
    seq_t f_2m = f_k * two_f_k1_minus_f_k;

    /* F(2m+1) = F(m)^2 + F(m+1)^2 */
    seq_t f_k_sq = f_k * f_k;
    seq_t f_k1_sq = f_k1 * f_k1;
    seq_t f_2m1 = f_k_sq + f_k1_sq;

    if ((k & (idx_t)1) == 0)
    {
        *fk  = f_2m;
        *fk1 = f_2m1;
    }
    else
    {
        *fk  = f_2m1;
        *fk1 = f_2m + f_2m1;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n + (idx_t)2, &fn, &fn1);   /* fn = Fibonacci(n+2) */
    return fn * ((seq_t)2) + (seq_t)3;    /* a(n) = 2*Fibonacci(n+2) + 3 */
}
