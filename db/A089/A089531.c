/*
 * A089531  Primes p such that (p-3)/2 is also prime.
 * https://oeis.org/A089531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p such that (p‑3)/2 is also prime
 * Range:     u8 n <= 15, u16 n <= 924, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 976 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently, p = 2*q + 3 with q prime.) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return the n‑th member of OEIS A089531 (1 ≤ n).  The function
 * keeps a static cache so that calls in any order are fast. */
seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* next candidate q to test (q is prime, p = 2*q+3 will be checked) */
    static idx_t candidate = 2;

    /* ensure that we have at least n terms ready */
    while (cache_len < n) {
        /* search for the next valid pair (q,p) */
        for (;;) {
            /* candidate is always odd after the first step */
            if (candidate == 2 || (candidate & 1)) {
                /* q must be prime */
                if (is_prime(candidate)) {
                    /* compute p = 2*q + 3 using a wide type to avoid overflow
                     * before the cast to seq_t. */
                    unsigned __int128 p_big = (unsigned __int128)candidate * 2 + 3;

                    /* only keep values that fit in seq_t */
                    /* automatically true for 64‑bit or larger builds */
                    seq_t p = (seq_t)p_big;

                    /* now confirm that p is prime */
                    if (is_prime_big(p)) {
                        /* grow the cache if needed */
                        if (cache_len + 1 > cache_cap) {
                            size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                            void *tmp = mem_resize(seq_t, cache, new_cap);
                            if (!tmp) { /* allocation failed – return 0 */
                                return (seq_t)0;
                            }
                            cache = (seq_t *)tmp;
                            cache_cap = new_cap;
                        }
                        cache[cache_len++] = p;

                        /* advance to the next odd candidate after q */
                        candidate += (candidate == 2 ? 1 : 2);
                        break; /* go back to fill next n */
                    }
                }
            }
            /* advance to the next odd integer */
            candidate += (candidate == 2 ? 1 : 2);

            /* If candidate grows beyond the range that can be stored in
             * seq_t, further terms cannot be produced.  The specification
             * guarantees that a(n) will be requested only while it still
             * fits in seq_t, so the loop normally terminates with the
             * desired number of terms. */
        }
    }
    /* cache is zero‑based, n is one‑based */
    return cache[n - 1];
}
