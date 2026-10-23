/*
 * A024899  Numbers k such that 6*k + 1 is prime.
 * https://oeis.org/A024899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024899: numbers k such that 6*k + 1 is prime.
   The function searches for successive ks, tests primality of 6*k+1,
   and memoizes the results to handle arbitrary call order. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* deterministic Miller‑Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* bases that are sufficient for n < 2^64 */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a, e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0;      /* number of terms stored */
    static size_t cap = 0;      /* capacity of allocated array */
    static idx_t last_k = 0;    /* last k examined */

    while (len < n) {
        /* search next candidate k */
        ++last_k;
        uint64_t p = 6ULL * last_k + 1;
        if (is_prime_uint64(p)) {
            /* add k to the stored sequence */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            seq[len++] = (seq_t)last_k;
        }
    }
    return seq[n - 1];
}
