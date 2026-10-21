/*
 * A075748  Numbers k such that 210*k-17 is prime.
 * https://oeis.org/A075748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth integer k such that 210*k-17 is prime
 * Range:     u8 n <= 109, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic prime test for 64‑bit numbers. */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    for (uint64_t i = 5; i <= x / i; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static idx_t last_k = 0;        /* largest k already checked       */
    static idx_t size   = 0;        /* number of cached terms          */
    static idx_t cap    = 0;        /* capacity of cache array         */
    static seq_t *cache = NULL;     /* cached k values                 */

    if (n == 0) return 0;           /* offset 1 – out of range           */

    while (size < n) {
        ++last_k;                        /* consider next k */

        uint64_t val = 210ull * (uint64_t)last_k - 17ull;
        if (is_prime(val)) {
            if (size + 1 > cap) {
                idx_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);                /* allocation failed */
                cache = tmp;
                cap   = new_cap;
            }
            cache[size++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
