/*
 * A043660  Numbers whose base-13 representation has exactly 5 runs.
 * https://oeis.org/A043660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑13 representation has exactly 5
 *            runs
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for the sequence.  20000 is safely larger than the first 10000
   terms; change if a larger n is required. */
static seq_t cache[20000];
static size_t cache_size = 0;      /* number of terms currently stored */
static seq_t next_val   = 1;       /* smallest value not yet examined */

/* Count the number of runs in the base‑13 representation of x.
   A run is a maximal block of equal consecutive digits. */
static int count_runs(seq_t x)
{
    if (x == 0) return 1;          /* base‑13 of 0 is "0" → one run */

    seq_t prev = x % 13;            /* least‑significant digit */
    x /= 13;
    int runs = 1;

    while (x > 0)
    {
        seq_t dig = x % 13;
        x /= 13;
        if (dig != prev)
        {
            ++runs;
            prev = dig;
        }
    }
    return runs;
}

/* Return the n‑th term of the sequence.  The function works for any
   order of calls; the cache is filled lazily. */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n)
    {
        seq_t val = next_val++;
        if (count_runs(val) == 5)
        {
            cache[cache_size++] = val;
        }
    }
    return cache[n - 1];
}
