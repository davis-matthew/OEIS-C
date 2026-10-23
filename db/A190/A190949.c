/*
 * A190949  Odd Fibonacci numbers with odd index.
 * https://oeis.org/A190949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-2) - a(n-4), with a(1)=1, a(2)=5, a(3)=13, a(4)=89
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef MAX_N
#define MAX_N 10000   /* Supports indices up to 10 000; larger values are not needed for the tests. */
#endif

static seq_t cache[MAX_N+1];
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term. */
    if (n == 0) return 0;   /* Defensive guard – not part of the sequence. */

    /* Initialise the first four values on first use. */
    if (max_computed == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)89;
        max_computed = 4;
    }

    /* Return the cached value if already computed. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Extend the cache up to the requested index. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The recurrence only requires terms that are at most four positions
           behind, so i-2 and i-4 are guaranteed to be valid.  We cast the
           integer constant 18 to seq_t to keep the operation type‑safe. */
        cache[i] = cache[i - 2] * (seq_t)18 - cache[i - 4];
    }

    max_computed = n;
    return cache[n];
}
