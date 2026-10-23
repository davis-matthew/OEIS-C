/*
 * A174853  y-values in the solution to x^2-47*y^2=1.
 * https://oeis.org/A174853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 96*a(n-1)-a(n-2); a(1)=0, a(2)=7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_CACHE_SIZE 10001

static seq_t cache[MAX_CACHE_SIZE];
static idx_t cache_next = 2;
static int init_flag = 0;

/* Return the n-th term of OEIS A174853 (y-values of x^2-47*y^2=1). */
seq_t a(idx_t n)
{
    /* Initialize cache with first two terms once. */
    if (!init_flag) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)7;
        cache_next = 2;
        init_flag = 1;
    }

    /* Handle trivial cases. */
    if (n == 0) return (seq_t)0;          /* unused, but safe */
    if (n <= 2) return cache[n];

    /* --------------------------------------------------------------------
       If the requested index is within the cache window, extend the cache
       up to n.  The cache is only valid up to MAX_CACHE_SIZE-1 (≈10000).
       -------------------------------------------------------------------- */
    if (n < MAX_CACHE_SIZE) {
        for (idx_t i = cache_next + 1; i <= n; ++i) {
            cache[i] = (seq_t)96 * cache[i-1] - cache[i-2];
        }
        cache_next = n;
        return cache[n];
    }

    /* --------------------------------------------------------------------
       For indices beyond the static cache, compute directly using a
       linear‑recurrence loop.  This keeps the function usable for very
       large n while still correct for all terms that fit in `seq_t`.
       -------------------------------------------------------------------- */
    seq_t a_prev2 = (seq_t)0;   /* a(1) */
    seq_t a_prev1 = (seq_t)7;   /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)96 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
