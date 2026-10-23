/*
 * A046128  Smallest side a of scalene integer Heronian triangles sorted by
 *          increasing c and b.
 * https://oeis.org/A046128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by searching for scalene integer Heronian triangles
 *            sorted by increasing c and b. Returns the smallest side a of the
 *            (n+1)-th such triangle
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- Helper: integer square root of a __int128 value -----------------*/
static inline unsigned long long isqrt_uint128(__int128 x) {
    unsigned long long low = 0;
    unsigned long long high = (unsigned long long)1 << 35; /* > sqrt(max t) */
    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;
        __int128 prod = (__int128)mid * mid;
        if (prod == x) return mid;
        if (prod < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/*--- Check whether a,b,c form a scalene integer Heronian triangle --*/
static inline bool is_heronian(uint64_t a, uint64_t b, uint64_t c) {
    __int128 t1 = (__int128)a + b + c;
    __int128 t2 = (__int128)b + c - a;
    __int128 t3 = (__int128)a + c - b;
    __int128 t4 = (__int128)a + b - c;
    __int128 area2 = t1 * t2;     /* (a+b+c)(b+c-a)(a+c-b)(a+b-c) */
    area2 *= t3;
    area2 *= t4;
    if (area2 <= 0) return false;
    if ((area2 & 15) != 0)        /* not divisible by 16  */
        return false;
    __int128 t = area2 >> 4;      /* area2 / 16  */
    unsigned long long r = isqrt_uint128(t);
    return (__int128)r * r == t;
}

/*--- Cache of computed a(n) values -----------------------------------*/
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Grow the cache to at least needed capacity */
static void grow_cache(size_t needed) {
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap < needed) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/*--- Generator state for incremental enumeration ----------------------*/
static uint64_t gen_c = 5;
static uint64_t gen_b = 0;
static uint64_t gen_a_start = 0;
static bool gen_init = false;

/*--- Main sequence function -------------------------------------------*/
seq_t a(idx_t n) {
    /* Ensure cache is large enough */
    while (cache_len <= (size_t)n) {
        /* Initialize generator if first time */
        if (!gen_init) {
            gen_c = 5;
            gen_b = gen_c / 2 + 1;
            gen_a_start = gen_c - gen_b + 1;
            gen_init = true;
        }

        /* Find next Heronian triangle */
        while (true) {
            if (gen_b >= gen_c) {            /* move to next c */
                ++gen_c;
                gen_b = gen_c / 2 + 1;
                continue;
            }
            if (gen_a_start > gen_b - 1) {    /* finished all a for this b */
                ++gen_b;
                gen_a_start = gen_c - gen_b + 1;
                continue;
            }

            /* test current (a,b,c) */
            uint64_t a_val = gen_a_start;
            if (is_heronian(a_val, gen_b, gen_c)) {
                /* store a in cache */
                if (cache_len == cache_cap)
                    grow_cache(cache_cap * 2);
                cache[cache_len++] = (seq_t)a_val;
                /* advance to next a for future iterations */
                ++gen_a_start;
                break;
            }
            /* not a Heronian triangle – advance to next a */
            ++gen_a_start;
        }
    }

    return cache[(size_t)n];
}
