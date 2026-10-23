/*
 * A142819  Primes congruent to 21 mod 61.
 * https://oeis.org/A142819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 21 (mod 61)
 * Range:     u8 none, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic primality test for 64‑bit integers */
static bool isprime_u64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    if (n % 3 == 0)           return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but protect against misuse */

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t cand = 21;   /* next number ≡ 21 (mod 61) to test */

    while (len < n) {
        if (cand >= 2 && isprime_u64(cand)) {
            if (len == cap) {            /* grow cache if needed */
                size_t newcap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = (seq_t)cand;   /* store prime */
        }
        cand += 61;                       /* next candidate in the progression */
    }
    return cache[n - 1];                  /* a(1) corresponds to index 0 */
}
