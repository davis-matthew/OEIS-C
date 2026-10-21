/*
 * A097621  In canonical prime factorization of n replace p^e with its index
 *          in A000961.
 * https://oeis.org/A097621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} A000961_index(p^e) where A000961_index(x) is
 *            the 1‑based index of the prime power x in the list A000961
 *            (1,2,3,4,5,7,8,9,11,…). For n = 1, a(1) = 1
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static int cmp_idx(const void *a, const void *b)
{
    const idx_t x = *(const idx_t*)a;
    const idx_t y = *(const idx_t*)b;
    return (x > y) - (x < y);
}

seq_t a(idx_t n)
{
    /* ---------- static tables ---------------------------------------- */
    static bool ready = false;
    static const idx_t INIT_LIMIT = 2000000;          /* generous upper bound   */
    static idx_t *spf = NULL;          /* smallest prime factor   */
    static idx_t *index_of = NULL;     /* index_of[prime_power]   */
    static size_t limit = 0;           /* current table size       */

    if (!ready) {
        limit = INIT_LIMIT;
        spf = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
        index_of = (idx_t*)calloc((limit + 1), sizeof(idx_t));

        /* sieve for smallest prime factor */
        for (idx_t i = 2; i <= limit; ++i) spf[i] = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {            /* i is prime */
                spf[i] = i;
                if (i <= limit / i) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        if (spf[j] == 0) spf[j] = i;
                }
            }
        }

        /* generate all prime powers <= limit */
        idx_t *pp_arr = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
        size_t pp_cnt = 0;
        pp_arr[pp_cnt++] = 1;            /* 1 is the first power */

        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == i) {           /* i is prime */
                idx_t p = i;
                idx_t val = p;
                while (val <= limit) {
                    pp_arr[pp_cnt++] = val;
                    if (val > limit / p) break;
                    val *= p;
                }
            }
        }

        /* sort and create index table */
        qsort(pp_arr, pp_cnt, sizeof(idx_t), cmp_idx);
        for (size_t i = 0; i < pp_cnt; ++i)
            index_of[pp_arr[i]] = (idx_t)(i + 1);

        free(pp_arr);
        ready = true;
    }

    /* ---------- compute a(n) ---------------------------------------- */
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* compute p^exp (fits in idx_t because <= limit) */
        idx_t val = 1;
        for (idx_t i = 0; i < exp; ++i) val *= p;

        /* multiply by the index of this prime power */
        idx_t idx_val = index_of[val];
        result *= idx_val;
    }

    return result;
}
