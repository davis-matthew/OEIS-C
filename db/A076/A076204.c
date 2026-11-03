/*
 * A076204  Numbers whose cube has a prime sum of digits.
 * https://oeis.org/A076204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   digit_sum, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076204: Numbers whose cube has a prime sum of digits.
   a(n) = the n‑th positive integer m such that digit_sum(m³) is prime. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* C: use malloc/free */
static inline seq_t* alloc_array(size_t count) { return mem_new(seq_t, count); }

static inline void free_array(seq_t* ptr) { mem_free(ptr); }

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static seq_t *cache   = NULL;   /* cached terms                         */
    static size_t  cap    = 0;      /* allocated capacity                    */
    static size_t  cnt    = 0;      /* number of cached terms                */
    static uint64_t last  = 0;      /* last integer 'm' processed            */
    static bool     init   = false; /* one‑time initialization of helpers     */

    /* prime table for sums up to 200 ----------------------------------- */
    static bool prime_sum[201];
    if (!init) {
        for (int i = 0; i <= 200; ++i) prime_sum[i] = false;
        prime_sum[2] = prime_sum[3] = prime_sum[5] = prime_sum[7] = true;
        for (int i = 11; i <= 200; i += 2) {
            bool p = true;
            for (int d = 3; d * d <= i; d += 2)
                if (i % d == 0) { p = false; break; }
            prime_sum[i] = p;
        }
        init = true;
    }

    /* generate terms until we have at least n entries ---------------- */
    while (cnt < n) {
        ++last;                      /* next integer m to test            */
        seq_t m  = (seq_t)last;      /* cast to seq_t                      */
        seq_t cube = m * m * m;      /* m³                                   */
        seq_t sum  = digit_sum(cube);
        idx_t sum_ul = (idx_t)sum;   /* the sum fits easily into idx_t      */

        if (sum_ul <= 200 && prime_sum[sum_ul]) {
            /* grow cache if necessary ---------------------------------- */
            if (cnt == cap) {
                size_t newcap = cap ? cap << 1 : 64;
                seq_t *newptr = alloc_array(newcap);
                if (!newptr) abort();          /* allocation failed          */
                for (size_t i = 0; i < cnt; ++i) newptr[i] = cache[i];
                if (cache) free_array(cache);
                cache = newptr;
                cap   = newcap;
            }
            cache[cnt++] = m;
        }
    }

    /* return the requested term (1‑based index) --------------------- */
    return cache[n - 1];
}
