/*
 * A126707  a(n) = number of composites coprime to n and <= the n-th
 *          composite.
 * https://oeis.org/A126707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of composites c ≤ C_n such that gcd(c,n)=1
   where C_n is the n‑th composite number (1‑based) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_idx_t.h"

/* ensure the dynamic array has at least 'need' elements */
static void ensure_capacity_uint64(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;         /* offset is 1 */

    /* static caches of primes and composites */
    static uint64_t *primes = NULL;
    static size_t primeCnt = 0, primeCap = 0;

    static uint64_t *composites = NULL;
    static size_t compCnt = 0, compCap = 0;

    static idx_t currentCandidate = 2;          /* next integer to test */

    /* generate primes and composites up to at least the n‑th composite */
    while (compCnt < n) {
        bool isPrime = true;
        for (size_t i = 0; i < primeCnt; ++i) {
            uint64_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > (uint64_t)currentCandidate) break;
            if (currentCandidate % p == 0) { isPrime = false; break; }
        }

        if (isPrime) {
            ensure_capacity_uint64(&primes, &primeCap, primeCnt + 1);
            primes[primeCnt++] = (uint64_t)currentCandidate;
        } else {
            ensure_capacity_uint64(&composites, &compCap, compCnt + 1);
            composites[compCnt++] = (uint64_t)currentCandidate;
        }
        ++currentCandidate;
    }

    /* n‑th composite number */
    uint64_t C_n = composites[n - 1];

    /* count composites ≤ C_n that are coprime to n */
    seq_t count = 0;
    for (size_t i = 0; i < compCnt; ++i) {
        if (composites[i] > C_n) break;
        if (gcd_idx_t(composites[i], n) == (idx_t)1) ++count;
    }
    return count;
}
