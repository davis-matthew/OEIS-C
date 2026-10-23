/*
 * A121891  Least m such that (F(n) mod m) > (F(n+2) mod m).
 * https://oeis.org/A121891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that (FibStd(n+1) mod m) > (FibStd(n+3) mod
 *            m), where FibStd is the standard Fibonacci sequence with
 *            FibStd(0)=0, FibStd(1)=1
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   fib_std_mod, mod_mul_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses fast‑doubling with safe modular multiplication. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/fib_std_mod.h"
#include "../utils/mod_mul_uint128.h"

seq_t a(idx_t n)
{
    /* Shift n by 1 so that we work with FibStd(n+1) and FibStd(n+3). */
    idx_t k = n + 1;                 /* k = n+1 */
    for (seq_t m = 2; ; ++m) {
        seq_t fn, fn1;
        fib_std_mod((idx_t)k, m, &fn, &fn1);   /* fn  = FibStd(k)   = FibStd(n+1)
                                                  fn1 = FibStd(k+1) = FibStd(n+2) */

        /* FibStd(n+3) = FibStd(n+1) + FibStd(n+2) mod m */
        seq_t fn3 = (fn + fn1) % m;

        if (fn > fn3) {
            return m;
        }
    }
}
