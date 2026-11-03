/*
 * A023860  a(n) = 1*t(n) + 2*t(n-1) + ... + k*t(n+1-k), where
 *          k=floor((n+1)/2), t = A000045 (Fibonacci numbers).
 * https://oeis.org/A023860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} i * t(n+1-i) , where t(n) is the n-th
 *            Fibonacci number (t(1)=1, t(2)=1)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 959 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Fibonacci numbers are computed within this file; no external
   helper is required. */
static seq_t* compute_fib_upto(idx_t n)
{
    /* Return an array fib[0..n] with fib[0]=0 and fib[1]=1. */
    seq_t* f = mem_new(seq_t, (n + 1));
    if (!f) return NULL;                 /* allocation failure -> caller handles 0 */
    f[0] = 0;
    if (n >= 1) f[1] = 1;
    for (idx_t i = 2; i <= n; ++i)
        f[i] = f[i - 1] + f[i - 2];
    return f;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1, values for n<1 are unused */
    /* Compute Fibonacci array up to n. */
    seq_t* fib = compute_fib_upto(n);
    if (!fib) return 0;                  /* on out‑of‑memory */
    seq_t sum = 0;
    idx_t k = (n + 1) / 2;               /* floor((n+1)/2) */
    for (idx_t i = 1; i <= k; ++i) {
        idx_t fib_index = n + 1 - i;     /* we need t(n+1-i) */
        sum += (seq_t)i * fib[fib_index];
    }
    mem_free(fib);
    return sum;
}
