/*
 * A061705  Number of matchings in the wheel graph with n spokes.
 * https://oeis.org/A061705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n+1)*Fibonacci(n) + 2*Fibonacci(n-1)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 39, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the fast–doubling Fibonacci helper
   fib_pair(idx_t n, seq_t *fn, seq_t *fn1) which returns
   fn = F(n) and fn1 = F(n+1). It works for any seq_t
   type (fixed‑size or big integer). */

#include "../utils/fib_pair.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, so this value is irrelevant */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);        /* fn  = F(n) , fn1 = F(n+1) */
    seq_t fn_minus1 = fn1 - fn;    /* since F(n+1) = F(n) + F(n-1)  */
    seq_t result  = fn * ((seq_t)(n + 1));
    result += fn_minus1 * ((seq_t)2);
    return result;
}
