/*
 * A103648  Expansion of -x*(2*x^11-2*x^9+3*x^7-2*x^6-x^5+x^3-x^2-1) /
 *          ((x-1)*(2*x^12+x^8-1)).
 * https://oeis.org/A103648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-8)-a(n-9)+2*a(n-12)-2*a(n-13)+c(n)
 * where c(n) is 0 except
 *  c(1)=1, c(3)=1, c(4)=-1, c(6)=1, c(7)=2, c(8)=-3, c(10)=2, c(12)=-2.
 * Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=1, a(5)=1,
 * a(6)=2, a(7)=4, a(8)=1. */
#include <stdlib.h>

/* Cached sequence values.  */
static seq_t *cache = NULL;
/* Number of elements allocated in cache.  */
static idx_t cached_size = 0;
/* Highest index that has been computed (inclusive).  */
static idx_t last_computed = 0;
/* Flag indicating whether the cache has been initialized.  */
static int initialized = 0;

/* Ensure that values up to and including n are available in cache.  */
static void compute_to(idx_t n)
{
    /* First time: allocate space for a(0) and set base value.  */
    if (!initialized) {
        if (cached_size < 1) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t));
            if (cache == NULL) abort();
            cached_size = 1;
        }
        cache[0] = (seq_t)0;
        last_computed = 0;
        initialized = 1;
    }

    /* If requested n is already computed, nothing to do.  */
    if (n <= last_computed) return;

    /* Grow the cache if necessary.  */
    if (n + 1 > cached_size) {
        size_t newsize = (size_t)(n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, newsize);
        if (tmp == NULL) abort();
        cache = tmp;
        cached_size = (idx_t)(n + 1);
    }

    /* Compute sequence values from last_computed + 1 up to n.  */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;

        /* Recurrence part.  */
        if (i >= 1)      val += cache[i - 1];
        if (i >= 8)      val += cache[i - 8];
        if (i >= 9)      val -= cache[i - 9];
        if (i >= 12)     val += (cache[i - 12] << 1);
        if (i >= 13)     val -= (cache[i - 13] << 1);

        /* Add the numerator coefficient c(i).  */
        switch (i) {
            case 1:  val += (seq_t)1;   break;
            case 3:  val += (seq_t)1;   break;
            case 4:  val -= (seq_t)1;   break;
            case 6:  val += (seq_t)1;   break;
            case 7:  val += (seq_t)2;   break;
            case 8:  val -= (seq_t)3;   break;
            case 10: val += (seq_t)2;   break;
            case 12: val -= (seq_t)2;   break;
            default: /* nothing */      break;
        }

        cache[i] = val;
    }

    last_computed = n;
}

seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
