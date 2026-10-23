/*
 * A168986  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 19*(a(n-1)+...+a(n-22)) - 190*a(n-23)  for n ≥ 24
   Initial values: a(0)=1, a(1)=21, a(2)=420, a(3)=8400, a(4)=168000,
   a(5)=3360000, a(6)=67200000, a(7)=1344000000, a(8)=26880000000,
   a(9)=537600000000, a(10)=10752000000000, a(11)=215040000000000,
   a(12)=4300800000000000, a(13)=86016000000000000, a(14)=1720320000000000000,
   a(15)=34406400000000000000, a(16)=688128000000000000000,
   a(17)=13762560000000000000000, a(18)=275251200000000000000000,
   a(19)=5505024000000000000000000, a(20)=110100480000000000000000000,
   a(21)=2202009600000000000000000000, a(22)=44040192000000000000000000000,
   a(23)=880803839999999999999999999790. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of a(0)..a(max) */
    static idx_t cache_size = 0;         /* allocated size */
    static idx_t computed_max = 0;       /* largest index computed so far */

    /* Ensure cache large enough for index n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_size = new_size;
    }

    /* If a(0) not yet initialized */
    if (cache_size > 0 && computed_max == 0 && n >= 0) {
        cache[0] = (seq_t)1;
        computed_max = 0;
    }

    /* Compute missing terms up to n */
    if (n > computed_max) {

        /* For 1 <= idx <= 23 compute using the generating‑function relation */
        for (idx_t idx = computed_max + 1; idx <= n; ++idx) {

            if (idx <= 22) {      /* a(idx) = 2 + 19 * sum_{k=0}^{idx-1} a(k) */
                seq_t sum = 0;
                for (idx_t k = 0; k < idx; ++k) sum += cache[k];
                cache[idx] = (seq_t)2 + (seq_t)19 * sum;
            }
            else if (idx == 23) { /* a(23) = 1 + 19 * sum_{k=1}^{22} a(23-k) - 190*a(0) */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 22; ++k) sum += cache[idx - k];
                cache[idx] = (seq_t)1 + (seq_t)19 * sum - (seq_t)190 * cache[0];
            }
            else {                /* idx >= 24 : homogeneous recurrence */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 22; ++k) sum += cache[idx - k];
                cache[idx] = (seq_t)19 * sum - (seq_t)190 * cache[idx - 23];
            }
        }
        computed_max = n;
    }

    return cache[n];
}
