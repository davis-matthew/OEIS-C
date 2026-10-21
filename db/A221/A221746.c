/*
 * A221746  Number of nX2 arrays of occupancy after each element moves to some
 *          king-move neighbor, without move-in move-out straight through or
 *          left turns.
 * https://oeis.org/A221746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +33*a(n-2) +7*a(n-3) -165*a(n-4) -152*a(n-5)
 *            -189*a(n-6) -572*a(n-7) -280*a(n-8) -223*a(n-9)
 *            -745*a(n-10)-574*a(n-11)-882*a(n-12)
 *            -773*a(n-13)-298*a(n-14)-128*a(n-15)
 *            +4*a(n-16)+19*a(n-17)+3*a(n-18) Initial values (offset 1):
 *            a(1)=1 a(2)=19 a(3)=98 a(4)=817 a(5)=5567 a(6)=41067 a(...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial terms needed for the recurrence; these are 18 values */
static const seq_t init_vals[18] = {
    1ULL,
    19ULL,
    98ULL,
    817ULL,
    5567ULL,
    41067ULL,
    293058ULL,
    2119375ULL,
    15244204ULL,
    109884658ULL,
    791399401ULL,
    5701648165ULL,
    41072174493ULL,
    295881007510ULL,
    2131463579726ULL,
    15354723297804ULL,
    110612658781667ULL,
    796834506416273ULL
};

/* dynamic cache for computed values */
static seq_t *cache = NULL;   /* indices 0..cache_cap-1 */
static size_t cache_cap = 0;
static idx_t cached_n   = 0;  /* highest n computed (minus 1, since cache[0] unused) */

/* Ensure the cache large enough to hold index n (1‑based).  */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation: indices 0..18 (19 slots) */
        cache_cap = 19;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort();
        /* fill in the 18 initial values */
        for (size_t i = 1; i <= 18; ++i)
            cache[i] = init_vals[i-1];
        cached_n = 18;
    }
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* newly allocated entries were zero‑initialized by realloc of larger size */
    }
}

/* Main function: returns a(n)  (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, safe guard */

    ensure_cache(n);

    if (n <= cached_n)
        return cache[n];

    /* Compute new values up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        sum += 3ULL * cache[i-1];
        sum += 33ULL * cache[i-2];
        sum += 7ULL * cache[i-3];
        sum -= 165ULL * cache[i-4];
        sum -= 152ULL * cache[i-5];
        sum -= 189ULL * cache[i-6];
        sum -= 572ULL * cache[i-7];
        sum -= 280ULL * cache[i-8];
        sum -= 223ULL * cache[i-9];
        sum -= 745ULL * cache[i-10];
        sum -= 574ULL * cache[i-11];
        sum -= 882ULL * cache[i-12];
        sum -= 773ULL * cache[i-13];
        sum -= 298ULL * cache[i-14];
        sum -= 128ULL * cache[i-15];
        sum += 4ULL * cache[i-16];
        sum += 19ULL * cache[i-17];
        sum += 3ULL * cache[i-18];
        cache[i] = sum;
    }

    cached_n = n;
    return cache[n];
}
