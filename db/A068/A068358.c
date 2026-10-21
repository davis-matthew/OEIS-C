/*
 * A068358  Numbers k such that the first digit of binomial(2k,k) is 1.
 * https://oeis.org/A068358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 9174, u32 n <= 9174, u64 n <= 9174,
 *            u128 n <= 9174, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068358: numbers k such that the first digit of binomial(2k,k) is 1.
   Computed by a linear scan of k, accumulating log10(C(2k,k)) to test the
   leading digit. */
#include <math.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;          // last processed k (starting from 0)
static long double curr_log = 0.0L; // log10(C(2k,k)) for current last_k
static const long double LOG10_2 = log10l(2.0L);
static const long double EPS = 1e-15L;

static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                // out of memory
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              // undefined for n < 1

    /* Initialise cache on first call */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)0;          // k = 0 gives C(0,0) = 1 -> leading digit 1
        cache_len = 1;
        last_k = 0;
        curr_log = 0.0L;
    }

    /* Generate terms up to requested index n */
    while (cache_len < (size_t)n) {
        idx_t k = last_k + 1;
        /* compute log10 of (2k*(2k-1))/(k*k) */
        long double term = log10l(((long double)(2*k)) *
                                  ((long double)(2*k-1)) /
                                  ((long double)k * (long double)k));
        curr_log += term;

        long double frac = curr_log - floorl(curr_log);
        if (frac + EPS < LOG10_2) {   // first digit is 1
            grow_cache(cache_len + 1);
            cache[cache_len] = (seq_t)k;
            cache_len++;
        }
        last_k = k;
    }

    return cache[n-1];
}
