/*
 * A088757  a(n) = index of the first occurrence of n in A088606.
 * https://oeis.org/A088757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A088606(m) = n, where A088606(m) is the
 *            least k such that concatenating k and prime(m) gives a prime.
 *            The code finds it by an incremental search over primes, memoized
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

static uint32_t *A088757_tab = NULL;
static size_t A088757_cap = 0;
static uint64_t A088757_m = 0;      /* number of primes processed */
static uint64_t A088757_p = 1;      /* last prime processed */

static void A088757_grow(size_t need)
{
    if (need < A088757_cap) return;
    size_t nc = A088757_cap ? A088757_cap : 1024;
    while (nc <= need) nc <<= 1;
    uint32_t *t = (uint32_t *)realloc(A088757_tab, nc * sizeof(uint32_t));
    if (!t) abort();
    memset(t + A088757_cap, 0, (nc - A088757_cap) * sizeof(uint32_t));
    A088757_tab = t;
    A088757_cap = nc;
}

static uint64_t A088757_k(uint64_t p)
{
    if (p == 2 || p == 5) return 0;
    uint64_t pw = 1, x = p;
    while (x > 0) { pw *= 10ULL; x /= 10; }
    for (uint64_t k = 1;; ++k) {
        if (is_prime((idx_t)(k * pw + p))) return k;
    }
}

seq_t a(idx_t n)
{
    A088757_grow((size_t)n + 1);
    while (A088757_tab[n] == 0) {
        uint64_t p = A088757_p + 1;
        if (A088757_p == 2) p = 3;
        else if (A088757_p > 2) p = A088757_p + 2;
        else p = 2;
        while (!is_prime((idx_t)p)) p += 2;
        A088757_p = p;
        A088757_m++;
        uint64_t v = A088757_k(p);
        if (v > 0) {
            A088757_grow((size_t)v + 1);
            if (A088757_tab[v] == 0) A088757_tab[v] = (uint32_t)A088757_m;
        }
    }
    return (seq_t)A088757_tab[n];
}
