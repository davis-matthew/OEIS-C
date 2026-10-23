/*
 * A019381  Primes with primitive root 57.
 * https://oeis.org/A019381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the nth prime p such that 57 is a primitive root modulo p
 *            (offset 1)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1 */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 1;     /* last integer examined */

    while (cache_sz < n) {
        /* find next prime */
        do {
            ++last_checked;
        } while (!is_prime(last_checked));

        /* primes 3 and 19 can never admit 57 as a primitive root
           because 57 ≡ 0 (mod 3,19) */
        if (last_checked == 3 || last_checked == 19)
            continue;

        uint64_t pm1 = (uint64_t)last_checked - 1;

        /* factor p-1 */
        uint64_t factors[64];
        uint64_t exps[64];
        size_t pf_cnt = 0;
        factor_u64(pm1, factors, exps, &pf_cnt);

        /* primitive‑root test */
        bool ok = true;
        for (size_t i = 0; i < pf_cnt && ok; ++i) {
            seq_t r = mod_pow((seq_t)57,
                              pm1 / factors[i],
                              (seq_t)last_checked);
            if (r == (seq_t)1)
                ok = false;
        }

        if (ok) {
            /* grow cache if needed */
            if (cache_sz >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)
                    return 0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
