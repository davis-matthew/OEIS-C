/*
 * A213996  Primes p=u^2+v^2 such that p+u or p+v is the next prime after p.
 * https://oeis.org/A213996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A213996: primes p = u² + v² such that p+u or p+v is the next prime after p */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"

/* External polymorphic integer types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
/* returns the n-th element of A213996 (offset 1) */
{
    if (n == 0) return 0; /* undefined for n < 1 */

    static seq_t *cache = NULL;          /* cached terms */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_checked = 1;    /* next prime will be >1 */

    /* compute terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* get next prime > last_checked */
        uint64_t p = next_prime(last_checked);
        last_checked = p;
        seq_t p_seq = (seq_t)p;

        /* next prime after p */
        uint64_t next_p = next_prime(p);
        seq_t nxt_seq = (seq_t)next_p;

        /* find representation p = u² + v² (unique for primes 2 or ≡1 mod 4) */
        uint64_t u_val = 0, v_val = 0;
        if (p == 2) {
            u_val = 1;
            v_val = 1;
        } else if (p % 4u == 1u) {
            seq_t sqrt_p = floor_sqrt(p_seq);
            uint64_t limit = (uint64_t)sqrt_p;
            for (uint64_t u = 1; u <= limit; ++u) {
                seq_t v2 = p_seq - (seq_t)(u * u);
                seq_t v = floor_sqrt(v2);
                if (v * v == v2) {
                    u_val = u;
                    v_val = (uint64_t)v;
                    break;
                }
            }
        }

        /* test the condition */
        bool ok = false;
        if (u_val != 0 || p == 2) {
            if ((p_seq + (seq_t)u_val) == nxt_seq ||
                (p_seq + (seq_t)v_val) == nxt_seq) {
                ok = true;
            }
        }

        if (ok) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_len = 0;
                    return 0; /* allocation failure */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p_seq;
        }
    }

    return cache[(size_t)n - 1];
}
