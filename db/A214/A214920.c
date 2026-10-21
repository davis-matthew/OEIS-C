/*
 * A214920  a(n) is the least m > 0 such that Fibonacci(n-m) divides
 *          Lucas(n+m).
 * https://oeis.org/A214920
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   addmod, lucas_mod_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A214920:
   a(n) = the least m>0 such that Fibonacci(n‑m) | Lucas(n+m)
   (offset 2).  The implementation uses only additions and a
   pre‑computed table of small Fibonacci numbers; it works for any
   seq_t (32/64/128‑bit or arbitrary precision) and fastens memoisation. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/addmod.h"
#include "../utils/lucas_mod_mod.h"

/* Precomputed Fibonacci numbers up to F(93), the largest that fits
   in a 64‑bit unsigned value.  They are used as moduli, so
   fibs[idx] == 0 indicates that Fib(idx) is too large to be used. */
static seq_t fibs[94];
static bool fibs_inited = false;

static void ensure_fibs(void)
{
    if (fibs_inited) return;
    fibs[0] = (seq_t)0;
    fibs[1] = (seq_t)1;
    for (int i = 2; i < 94; ++i)
        fibs[i] = fibs[i - 1] + fibs[i - 2];
    fibs_inited = true;
}

/* Main sequence function.  The function is memoised per call,
   but the actual computation is O(n^2) in the worst case.  The
   loop stops early when a valid m is found.  For n<2 the result
   is undefined, but we return 0 to avoid UB. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    ensure_fibs();

    for (idx_t m = 1; m < n; ++m) {
        idx_t idx = n - m;                 /* index for Fibonacci */
        if (idx >= 94)                     /* Fib(idx) too big to use */
            continue;
        seq_t mod = fibs[idx];
        if (mod == 0) continue;            /* skip invalid modulus */

        seq_t rem = lucas_mod_mod(mod, n + m);
        if (rem == 0) return (seq_t)m;     /* found smallest m */
    }
    return (seq_t)0; /* should never happen for n>=2 */
}
