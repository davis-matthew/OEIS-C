/*
 * A182167  Min( f(n), n-f(n) ), where f(n) = A002708(n) = Fibonacci(n) mod n.
 * https://oeis.org/A182167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(Fibonacci(n) mod n, n - (Fibonacci(n) mod n))
 * Range:     u8 n <= 523, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety; not used in the offset */
    if (n == 1) return 0;            /* Fibonacci(1) = 0 */

    idx_t a0 = 0, a1 = 1;             /* F(1) mod n, F(2) mod n */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        /* compute (a0 + a1) % n safely using 128‑bit arithmetic */
        unsigned __int128 sum = (unsigned __int128)a0 + a1;
        idx_t tmp = (idx_t)(sum % n);
        a0 = a1;
        a1 = tmp;
    }

    idx_t fib_mod = a1;                 /* Fibonacci(n) mod n */

    seq_t mod_val = (seq_t)fib_mod;
    seq_t n_val   = (seq_t)n;
    seq_t diff    = n_val - mod_val;    /* if fib_mod == 0, diff == n_val */

    return (mod_val <= diff) ? mod_val : diff;
}
