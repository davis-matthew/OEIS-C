/*
 * A129307  Intersection of A000217 and A005098.
 * https://oeis.org/A129307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A027861(n)*(A027861(n)+1)/2 * A027861(n) is the n-th
 *            integer m such that m^2+(m+1)^2 is prime (i.e. 2m(m+1)+1 is
 *            prime). * a(n) is then the triangular number T(m)=m(m+1)/2
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

static idx_t *m_cache = NULL;      /* stores the m-values of A027861 */
static size_t m_cache_cap = 0;     /* capacity of m_cache */
static size_t m_cache_len = 0;     /* number of stored elements */

static void
ensure_m_cache(idx_t needed)
{
    if (m_cache_len >= needed)
        return;

    idx_t cur = (m_cache_len == 0) ? 1 : m_cache[m_cache_len - 1] + 1;

    while (m_cache_len < needed) {
        /* test if 2*m*(m+1)+1 is prime */
        seq_t val = ((seq_t)2 * (seq_t)cur * (seq_t)(cur + 1)) + 1;
        if (is_prime_big(val)) {
            ensure_capacity(m_cache_len + 1, &m_cache_cap,
                            (void **)&m_cache, sizeof(idx_t));
            m_cache[m_cache_len++] = cur;
        }
        cur++;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 for safety */

    ensure_m_cache(n);

    seq_t m = (seq_t)m_cache[n - 1];   /* A027861(n) */
    return (m * (m + 1)) / 2;
}
