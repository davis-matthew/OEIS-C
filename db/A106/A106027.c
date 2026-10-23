/*
 * A106027  Smallest m > n such that the Zeckendorf partition of m contains
 *          that of n.
 * https://oeis.org/A106027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A106027: the smallest m > n whose Zeckendorf partition
   contains that of n. */
#include <stdbool.h>
#include <stdint.h>

/* Maximum number of Fibonacci indices needed for the intended input range.
   64 is more than enough for n <= 10 000 (the Zeckendorf representation
   uses at most ~20 Fibonacci numbers). */
#define MAX_FIB 64

static seq_t fibs[MAX_FIB];
static idx_t fib_len = 0;

/* Lazily initialise the Fibonacci table, extending it until the
   largest stored Fibonacci number exceeds the given limit. */
static void ensure_fibs(idx_t limit)
{
    if (fib_len == 0) {
        fibs[0] = (seq_t)1;
        fibs[1] = (seq_t)2;
        fib_len = 2;
    }
    while (fib_len < MAX_FIB && (seq_t)fibs[fib_len-1] <= (seq_t)limit) {
        fibs[fib_len] = fibs[fib_len-1] + fibs[fib_len-2];
        ++fib_len;
    }
}

/* Return a 64‑bit mask of the Zeckendorf representation of x.
   Bit i is set iff Fibonacci number fibs[i] is used. */
static uint64_t zeck_mask(seq_t x)
{
    uint64_t mask = 0;
    for (idx_t i = fib_len; i-- > 0;) {
        if (x >= fibs[i]) {
            x -= fibs[i];
            mask |= (uint64_t)1u << i;
        }
    }
    return mask;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, return 0 */

    ensure_fibs(n);
    uint64_t mask_n = zeck_mask((seq_t)n);

    for (idx_t m = n + 1; ; ++m) {
        ensure_fibs(m);
        if ((zeck_mask((seq_t)m) & mask_n) == mask_n)
            return (seq_t)m;
    }
}
