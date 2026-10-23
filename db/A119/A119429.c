/*
 * A119429  Expansion of Sum_{k>=0} x^(2*k)/Product_{j=1..k} (1 - j*2*x).
 * https://oeis.org/A119429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119429
   a(n) = Sum_{k=0..⌊n/2⌋} S2(n-k,k) * 2^(n-2k)
   S2 are Stirling numbers of the second kind.
   Using the recurrence
      f(n,k) = S2(n-k,k) * 2^(n-2k)
   we obtain
      f(n,k) = 2*k * f(n-1,k) + f(n-2,k-1)         (n>=2k, k>0)
   with f(0,0)=1 and f(n,0)=0 for n>0.
   a(n) = Σ_k f(n,k).
   The implementation keeps a small rolling cache of the last two
   diagonals of f and memoises all a(n) values.  No global memory
   beyond the caches is required, and the algorithm runs in O(n^2)
   time and O(n) memory, well within the limits for n≈10 000. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for a(n) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed_max = 0;

    /* static buffers for three consecutive diagonals of f */
    static seq_t *f_prevprev = NULL;
    static size_t sz_prevprev = 0;   /* size of f_prevprev  */
    static seq_t *f_prev    = NULL;
    static size_t sz_prev    = 0;   /* size of f_prev     */
    static seq_t *f_curr    = NULL;
    static size_t sz_curr    = 0;   /* size of f_curr     */

    /* ensure cache can hold up to n */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        if (new_size > cache_size) {
            /* initialise new area to zero */
            memset(&tmp[cache_size], 0, (new_size - cache_size) * sizeof(seq_t));
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* initialisation for n = 0 */
    if (computed_max == 0 && cache[0] == 0) {
        cache[0] = (seq_t)1;             /* a(0) = 1 */

        /* f_prevprev is unused for n=0 */
        f_prevprev = NULL;
        sz_prevprev = 0;

        /* f_prev holds f(0,0) */
        f_prev = (seq_t *)malloc(sizeof(seq_t));
        if (!f_prev) abort();
        f_prev[0] = (seq_t)1;
        sz_prev = 1;

        f_curr = NULL;
        sz_curr = 0;
        computed_max = 0;
    }

    /* compute values up to n, if necessary */
    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        size_t kmax = (size_t)cur / 2;              /* floor(cur/2) */
        /* allocate current diagonal */
        f_curr = (seq_t *)calloc(kmax + 1, sizeof(seq_t));
        if (!f_curr) abort();

        /* compute f(cur,k) for k = 0..kmax */
        for (size_t k = 0; k <= kmax; ++k) {
            if (k == 0) {
                f_curr[0] = (cur == 0) ? (seq_t)1 : (seq_t)0;
            } else {
                seq_t term1 = 0, term2 = 0;
                /* first part: 2*k * f(prev, k)  if cur >= 2*k */
                if (cur >= 2 * k) {
                    if (k < sz_prev) {
                        term1 = (seq_t)(2 * k) * f_prev[k];
                    }
                }
                /* second part: f(prevprev, k-1)  if cur >= 2*k */
                if (cur >= 2 * k) {
                    if (f_prevprev && (k - 1) < sz_prevprev) {
                        term2 = f_prevprev[k - 1];
                    }
                }
                f_curr[k] = term1 + term2;
            }
        }

        /* a(cur) = sum_k f(cur,k) */
        seq_t sum = 0;
        for (size_t k = 0; k <= kmax; ++k) sum += f_curr[k];
        cache[cur] = sum;

        /* rotate diagonals for next iteration */
        free(f_prevprev);
        f_prevprev = f_prev;
        sz_prevprev = sz_prev;
        f_prev = f_curr;
        sz_prev = sz_curr;          /* current size was kmax+1, stored below */
        sz_prev = kmax + 1;
        f_curr = NULL;
        sz_curr = 0;

        ++computed_max;
    }

    return cache[n];
}
