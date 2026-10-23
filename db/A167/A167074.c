/*
 * A167074  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A167074.
   Recurrence:
     a(0) = 1
     a(1) = 21
     a(2) = 420
     a(3) = 8400
     a(4) = 168000
     a(5) = 3360000
     a(6) = 67200000
     a(7) = 1344000000
     a(8) = 26880000000
     a(9) = 537600000000
     a(10)= 10752000000000
     a(11)= 215040000000000
     a(12)= 4300800000000000
   For n ≥ 13:
     a(n) = 19*∑_{k=1}^{12} a(n‑k) – 190*a(n‑13)
   The numerator of the generating function gives a special
   adjustment for n = 13:
     a(13) = 19*∑_{k=1}^{12} a(13‑k) – 190*a(0) + 1
   The recurrence holds for all larger n, and the sequence is
   uniquely determined by these initial values and relations.  */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    (seq_t)1ULL,          /* a(0) */
    (seq_t)21ULL,         /* a(1) */
    (seq_t)420ULL,        /* a(2) */
    (seq_t)8400ULL,       /* a(3) */
    (seq_t)168000ULL,     /* a(4) */
    (seq_t)3360000ULL,    /* a(5) */
    (seq_t)67200000ULL,   /* a(6) */
    (seq_t)1344000000ULL, /* a(7) */
    (seq_t)26880000000ULL,/* a(8) */
    (seq_t)537600000000ULL,/* a(9) */
    (seq_t)10752000000000ULL,/* a(10) */
    (seq_t)215040000000000ULL,/* a(11) */
    (seq_t)4300800000000000ULL/* a(12) */
};

seq_t a(idx_t n)
{
    /* Simple dynamic programming cache */
    static seq_t *cache = NULL;          /* allocated array */
    static idx_t cache_len = 0;          /* number of terms already known */

    /* Ensure the cache has room for at least n + 1 terms */
    if (n >= cache_len) {
        idx_t new_cap = cache_len ? (cache_len << 1) : 64;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out‑of‑memory */
        cache = tmp;

        /* Compute new terms from cache_len up to n */
        idx_t i;
        for (i = cache_len; i <= n; ++i) {
            if (i < 13) {                       /* direct initial values */
                cache[i] = init_vals[i];
            } else if (i == 13) {               /* special case with +1 */
                seq_t sum = 0;
                idx_t k;
                for (k = 1; k <= 12; ++k) sum += cache[i - k];
                cache[i] = 19 * sum - 190 * cache[0] + 1;
            } else {                            /* recurrence for n >= 14 */
                seq_t sum = 0;
                idx_t k;
                for (k = 1; k <= 12; ++k) sum += cache[i - k];
                cache[i] = 19 * sum - 190 * cache[i - 13];
            }
        }
        cache_len = new_cap;                    /* reserve that capacity */
        cache_len = i;                          /* update known terms */
    }

    return cache[n];
}
