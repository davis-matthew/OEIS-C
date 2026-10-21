/*
 * A077103  Numbers n such that gcd(a,b) is not equal to gcd(a+b,a-b), where
 *          a=sigma(n)=A000203(n) and b=phi(n)=A000010(n).
 * https://oeis.org/A077103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A077103 – numbers n such that
    gcd(sigma(n),phi(n)) ≠ gcd(sigma(n)+phi(n), sigma(n)-phi(n)).
    The function returns the n‑th such number.
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* cache of the discovered terms of the sequence */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t cand = 1;               /* next integer to test */

    while (cache_len < (size_t)n) {
        seq_t sigma_val = sigma_of(cand);
        seq_t phi_val   = phi(cand);

        seq_t g1 = gcd(sigma_val, phi_val);

        seq_t sum = sigma_val + phi_val;
        seq_t diff = sigma_val > phi_val ? sigma_val - phi_val : phi_val - sigma_val;
        seq_t g2 = gcd(sum, diff);

        if (g1 != g2) {
            /* grow the cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* Out‑of‑memory: terminate */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
        cand++;
    }

    return cache[n - 1];
}
