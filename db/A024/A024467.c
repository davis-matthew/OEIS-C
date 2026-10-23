/*
 * A024467  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (Fibonacci numbers), t = A014306.
 * https://oeis.org/A024467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} F(i) * t(n+1-i) where F(i) is the
 *            i‑th Fibonacci number (F(1)=1, F(2)=1) and t(m)=0 if m = C(r,3)
 *            for some r≥3, t(m)=1 otherwise
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_binomial_c3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_binomial_c3.h"

/* Fibonacci cache: fib_cache[k] = F(k)
   fib_cache[0] is unused (set 0 for safety) */
static seq_t *fib_cache = NULL;
static idx_t fib_cap = 0;   /* maximum index stored, inclusive */

/* Ensure fib_cache contains Fibonacci numbers up to index k (inclusive) */
static void ensure_fib(idx_t k)
{
    if (k <= fib_cap) return;          /* already have enough */

    /* Allocate new storage for indices 0..k inclusive */
    seq_t *new_fib = (seq_t *)realloc(fib_cache, (size_t)(k + 1) * sizeof(seq_t));
    if (!new_fib) abort(); /* allocation failure */

    /* Initialise base cases if they were missing */
    if (fib_cap == 0) {
        new_fib[0] = (seq_t)0;
        if (k >= 1) new_fib[1] = (seq_t)1;
        fib_cap = 1;
        /* start computing from index 2 onwards */
        fib_cap = 1;
        if (k < 2) { fib_cache = new_fib; return; }
        for (idx_t i = 2; i <= k; ++i)
            new_fib[i] = new_fib[i-1] + new_fib[i-2];
    } else {
        /* fib_cache already has values up to fib_cap.
           Compute further values if needed. */
        for (idx_t i = fib_cap + 1; i <= k; ++i)
            new_fib[i] = new_fib[i-1] + new_fib[i-2];
        fib_cap = k;
    }

    fib_cache = new_fib;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n=0, return 0 */

    idx_t k = (n + 1) / 2;          /* floor((n+1)/2) */
    ensure_fib(k);                 /* make sure Fibonacci numbers are ready */

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t j = n + 1 - i;       /* index used for t */
        if (!is_binomial_c3(j))
            sum += fib_cache[i];
    }
    return sum;
}
