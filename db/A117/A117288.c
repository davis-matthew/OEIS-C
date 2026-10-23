/*
 * A117288  Numbers k for which the cototient k-phi(k) is an octagonal number.
 * https://oeis.org/A117288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 6847, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117288: numbers k such that k - φ(k) is an octagonal number.
   The function computes the n‑th term by scanning successive integers,
   storing each valid k in a static cache until at least n terms are
   found.  No floating point or pre‑computed table is used.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t isqrt_u64(uint64_t n)
{
    /* integer square‑root, 0 ≤ n < 2⁶⁴.  Uses binary search. */
    uint64_t lo = 0, hi = (uint64_t)1 << 32; /* sqrt(2⁶⁴‑1) < 2³² */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)   /* mid² ≤ n, avoid overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static bool is_octagonal(uint64_t d)
{
    if (d == 0)           /* 0 = n(3n‑2) with n = 0 */
        return true;

    uint64_t D = 4 + 12 * d;                   /* discriminant */
    uint64_t r = isqrt_u64(D);
    if ((unsigned __int128)r * r != D)        /* not a perfect square */
        return false;

    return ((2 + r) % 6) == 0;                 /* n = (2+r)/6 integer? */
}

static uint64_t phi_uint64(uint64_t m)
{
    if (m < 2)
        return 1;   /* φ(1)=1, φ(0) not used here */

    uint64_t result = m;
    uint64_t n = m;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            result -= result / p;
        }
    }
    if (n > 1)          /* last prime factor */
        result -= result / n;
    return result;
}

/* dynamic array helpers ----------------------------------------------------- */
static void ensure_cache_cap(size_t needed, seq_t **buf, size_t *cap)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*buf, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    *buf = tmp;
    *cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/* Main function: returns the n‑th term of A117288.                              */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static idx_t current_k = 1;     /* next integer to examine          */
    static size_t seq_len = 0;      /* number of terms found so far     */
    static size_t seq_cap = 0;      /* capacity of cache                */
    static seq_t *seq_cache = NULL; /* array holding found terms        */

    while (seq_len < n) {
        uint64_t k = current_k;
        uint64_t phi_val = phi_uint64(k);
        uint64_t diff = k - phi_val;

        if (is_octagonal(diff)) {
            ensure_cache_cap(seq_len + 1, &seq_cache, &seq_cap);
            seq_cache[seq_len++] = (seq_t)k;
        }
        ++current_k;
    }

    return seq_cache[n - 1];
}
