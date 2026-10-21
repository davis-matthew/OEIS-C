/*
 * A076988  Smallest Fibonacci number of the form n*k + 1 with k>0.
 * https://oeis.org/A076988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 24, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A076988: smallest Fibonacci number of the form n*k+1 with k>0 */
seq_t a(idx_t n)
{
    /* Special case: n = 1 → a(1) = 2 */
    if (n == 1) {
        return (seq_t)2;
    }

    /* Maintain Fibonacci sequence values (seq_t) and their remainders modulo n (idx_t). */
    idx_t mod_prev = 1 % n;           /* F(1) mod n */
    idx_t mod_curr = 1 % n;           /* F(2) mod n */
    idx_t idx     = 2;                /* current Fibonacci index */

    seq_t fib_prev = (seq_t)1;        /* F(1) */
    seq_t fib_curr = (seq_t)1;        /* F(2) */

    while (1) {
        /* Compute next Fibonacci number */
        idx++;
        seq_t fib_next = fib_prev + fib_curr;
        idx_t  mod_next = (mod_prev + mod_curr) % n;   /* (F(k-1)+F(k)) mod n */

        /* Check the defining condition */
        if (mod_next == 1 && idx >= 3) {
            return fib_next;
        }

        /* Shift for next iteration */
        fib_prev = fib_curr;
        fib_curr = fib_next;
        mod_prev = mod_curr;
        mod_curr = mod_next;
    }
}
