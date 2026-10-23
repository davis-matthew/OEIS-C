/*
 * A024872  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers >= 2), t = (Fibonacci numbers).
 * https://oeis.org/A024872
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A024872
    a(n) = Σ_{i=1..⌊n/2⌋} (i+1) * F(n-i+1)
    where F(1)=1, F(2)=1, F(k)=F(k-1)+F(k-2)
*/

#include <stdlib.h>
#include <stdint.h>

/* cache of Fibonacci numbers, 1‑indexed.  F[0] unused. */
static seq_t *fib_cache = NULL;   /* pointer to array of seq_t */
static size_t fib_len = 0;         /* current largest index stored (≥2 when initialized) */

/* Ensure fib_cache contains Fibonacci numbers up to at least index n (1‑based). */
static void ensure_fib(idx_t n)
{
    /* No work needed if already computed. */
    if (fib_len >= (size_t)n)
        return;

    /* Allocate space for indices up to n (inclusive). */
    size_t target = (size_t)n;
    /* Allocate at least 4 positions to avoid many reallocs for small n. */
    if (target < 4)
        target = 4;

    seq_t *tmp = (seq_t *)realloc(fib_cache, sizeof(seq_t) * (target + 1));
    if (!tmp)
        abort();  /* allocation failure */

    /* If this is the first allocation, initialise the first two Fibonacci numbers. */
    if (!fib_cache) {
        tmp[0] = 0;          /* unused index */
        tmp[1] = 1;          /* F(1) */
        tmp[2] = 1;          /* F(2) */
        fib_len = 2;
    }

    /* Generate any new Fibonacci entries that are required. */
    for (size_t i = fib_len + 1; i <= target; ++i) {
        tmp[i] = tmp[i - 1] + tmp[i - 2];
    }

    fib_cache = tmp;
    fib_len   = target;
}

/* Compute a(n) for the sequence A024872.  Calls are independent and may
   be in any order.  For n < 2 the return value is undefined (offset is 2). */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* behaviour below offset is irrelevant */

    ensure_fib(n);            /* make sure Fibonacci numbers up to n exist */

    size_t k = (size_t)(n / 2);   /* floor(n/2) */
    seq_t sum = 0;

    for (size_t i = 1; i <= k; ++i) {
        seq_t s = (seq_t)(i + 1);                        /* s(i) = i+1 */
        seq_t t = fib_cache[(size_t)n - i + 1];          /* t(n-i+1) = F(n-i+1) */
        sum += s * t;
    }

    return sum;
}
