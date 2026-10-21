/*
 * A205434  Number of (n+1)X3 0..3 arrays with every 2X2 subblock having the
 *          same number of equal edges, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A205434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence of order 11:
   a(1)=77, a(2)=833, a(3)=10275, a(4)=140423, a(5)=2052727,
   a(6)=31378965, a(7)=493169259, a(8)=7881048447,
   a(9)=127174581623, a(10)=2063732356701, a(11)=33596865282059;
   for n>11:
     a(n)= 45*a(n-1)-775*a(n-2)+6501*a(n-3)-27165*a(n-4)
           +43819*a(n-5)+59551*a(n-6)-369405*a(n-7)
           +633750*a(n-8)-521280*a(n-9)+206064*a(n-10)-31104*a(n-11) */

#include <stdlib.h>
#include <stdbool.h>

static idx_t cache_size = 0;          /* largest index computed so far */
static seq_t *cache = NULL;           /* cache[0] unused, cache[i] = a(i) for 1<=i<=cache_size */

/* initial values for a(1)..a(11); index 0 unused */
static const seq_t init_vals[12] = {
    (seq_t)0,
    (seq_t)77ULL,
    (seq_t)833ULL,
    (seq_t)10275ULL,
    (seq_t)140423ULL,
    (seq_t)2052727ULL,
    (seq_t)31378965ULL,
    (seq_t)493169259ULL,
    (seq_t)7881048447ULL,
    (seq_t)127174581623ULL,
    (seq_t)2063732356701ULL,
    (seq_t)33596865282059ULL
};

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;          /* already computed */

    size_t new_size = (size_t)n + 1;       /* include index 0 */

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_size = 0;
    } else if ((size_t)cache_size < new_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 11) {
            cache[i] = init_vals[i];
        } else {
            seq_t val = (seq_t)45   * cache[i-1];
            val -= (seq_t)775  * cache[i-2];
            val += (seq_t)6501 * cache[i-3];
            val -= (seq_t)27165* cache[i-4];
            val += (seq_t)43819* cache[i-5];
            val += (seq_t)59551* cache[i-6];
            val -= (seq_t)369405*cache[i-7];
            val += (seq_t)633750*cache[i-8];
            val -= (seq_t)521280*cache[i-9];
            val += (seq_t)206064*cache[i-10];
            val -= (seq_t)31104 *cache[i-11];
            cache[i] = val;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
