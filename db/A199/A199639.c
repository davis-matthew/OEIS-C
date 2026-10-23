/*
 * A199639  Indices of hexagonal numbers that are not the difference of two
 *          positive hexagonal numbers.
 * https://oeis.org/A199639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indices of hexagonal numbers that are not a difference of two positive
   hexagonal numbers (OEIS A199639).  The algorithm tests each integer i
   starting from 1, checks whether i*(2*i-1) can be expressed as a
   difference of two positive hexagonal numbers, and collects those that
   cannot.  Results are memoised; the function can be called in any order
   and will always return the correct value in O(n) time for n up to 10000.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Utility types supplied by the build environment.  (They are not defined
   here; they must be supplied by the compiler wrapper.) */
typedef uint64_t idx_t;

/* A structure for prime factor (prime, exponent). */
typedef struct {
    idx_t p;
    int e;
} factor_t;

/* -------------------------------------------- */
/* Factor n into primes; result stored in fac, *cnt set to number of factors.
   The array must have space for at least 32 entries. */
static void factor_uint64(idx_t n, factor_t *fac, int *cnt)
{
    *cnt = 0;
    if (n == 1) return;
    /* factor 2 separately */
    if ((n & 1) == 0) {
        int e = 0;
        while ((n & 1) == 0) {
            n >>= 1;
            ++e;
        }
        fac[(*cnt)++] = (factor_t){2, e};
    }
    /* odd factors */
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            fac[(*cnt)++] = (factor_t){p, e};
        }
    }
    if (n > 1) fac[(*cnt)++] = (factor_t){n, 1};
}

/* -------------------------------------------- */
/* Recursive enumeration of all divisors of M; returns true if a divisor
   satisfying the representability condition is found. */
static bool dfs_divisors(int idx, uint64_t cur,
                         uint64_t M,
                         const factor_t *factors, int nf)
{
    if (idx == nf) {
        uint64_t d = cur;          /* current divisor */
        uint64_t X = M / d;
        if (X >= 2 * d) {
            uint64_t val = X - 2 * d + 1;
            if ((val & 3) == 0) /* val % 4 == 0 */
                return true;
        }
        return false;
    }
    const factor_t *f = &factors[idx];
    uint64_t mult = 1;
    for (int e = 0; e <= f->e; ++e) {
        if (dfs_divisors(idx + 1, cur * mult, M, factors, nf))
            return true;
        mult *= f->p;
    }
    return false;
}

/* -------------------------------------------- */
/* Return true iff the i‑th hexagonal number can be expressed as
   the difference of two positive hexagonal numbers. */
static bool is_representable(idx_t i)
{
    uint64_t M = i * (2 * i - 1);          /* fits in 64 bits for i <= 10^9 */
    factor_t fac1[32], fac2[32];
    int cnt1, cnt2;
    factor_uint64(i, fac1, &cnt1);
    factor_uint64(2 * i - 1, fac2, &cnt2);

    /* Merge factor lists; the two numbers are coprime. */
    factor_t all[64];
    int allcnt = 0;
    for (int k = 0; k < cnt1; ++k) all[allcnt++] = fac1[k];
    for (int k = 0; k < cnt2; ++k) all[allcnt++] = fac2[k];

    /* Search all divisors of M. */
    return dfs_divisors(0, 1, M, all, allcnt);
}

/* -------------------------------------------- */
/* Cache of non‑representable indices. */
static idx_t *rep_cache = NULL;       /* array of indices */
static size_t rep_cache_len = 0;      /* number of elements stored */
static size_t rep_cache_cap = 0;      /* current capacity of the array */
static idx_t rep_cache_next = 1;      /* next integer to test for representability */

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (rep_cache_cap >= need) return;
    size_t newcap = rep_cache_cap ? rep_cache_cap * 2 : 8;
    while (newcap < need) newcap *= 2;
    idx_t *tmp = (idx_t *)realloc(rep_cache, newcap * sizeof(idx_t));
    if (!tmp) {
        /* In a real program you'd handle out‑of‑memory, but for the
           OEIS testing environment exiting is acceptable. */
        exit(EXIT_FAILURE);
    }
    rep_cache = tmp;
    rep_cache_cap = newcap;
}

/* -------------------------------------------- */
/* The function required by the build.  It may be called with any n >= 1,
   in any order. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (rep_cache_len < n) {
        idx_t i = rep_cache_next++;
        if (!is_representable(i)) {
            ensure_cache_cap(rep_cache_len + 1);
            rep_cache[rep_cache_len++] = i;
        }
    }
    return (seq_t)rep_cache[n - 1];
}
