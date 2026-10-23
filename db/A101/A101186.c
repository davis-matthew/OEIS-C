/*
 * A101186  Values of k for which 7m+1, 8m+1 and 11m+1 are prime, with m =
 *          1848k + 942.
 * https://oeis.org/A101186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that m = 1848·k + 942 yields the
 *            primes 7m+1, 8m+1, 11m+1. The function searches k in increasing
 *            order, testing each candidate with a deterministic 64‑bit
 *            Miller–Rabin and memoises the results for efficient repeated
 *            calls
 * Range:     u8 n <= 4, u16 n <= 328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modpow_u64, mulmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/modpow_u64.h"
#include "../utils/mulmod_u64.h"

static bool isPrime64(uint64_t n)
{
    if (n < 2ULL) return false;
    /* trial division by small primes */
    static const uint64_t small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 = d·2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1U;
        ++s;
    }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = modpow_u64(a, d, n);
        if (x == 1ULL || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mulmod_u64(x, x, n);
            if (x == n-1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}
/* ------------------------------------------------------------------ */

/* memoised list of found k's */
static seq_t *k_list = NULL;
static size_t k_len   = 0;
static size_t k_cap   = 0;
static uint64_t next_candidate = 1;  /* next k to test */

/* ensure capacity of k_list */
static void ensure_cap(size_t needed)
{
    if (k_cap >= needed) return;
    size_t new_cap = k_cap == 0 ? 16 : k_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(k_list, new_cap * sizeof(seq_t));
    if (!tmp) return;           /* silently ignore allocation failure */
    k_list = tmp;
    k_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined; return 0 for safety */
    size_t target = (size_t)n;
    if (target <= k_len) return k_list[target-1];

    while (k_len < target)
    {
        uint64_t k = next_candidate++;
        uint64_t m = 1848ULL * k + 942ULL;
        uint64_t p1 = 7ULL * m + 1ULL;
        uint64_t p2 = 8ULL * m + 1ULL;
        uint64_t p3 = 11ULL * m + 1ULL;
        if (isPrime64(p1) && isPrime64(p2) && isPrime64(p3))
        {
            ensure_cap(k_len + 1);
            k_list[k_len++] = (seq_t)k;
        }
    }
    return k_list[target-1];
}
