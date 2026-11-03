/*
 * A173229  a(n) is the n-th number m such that 6m-1 is composite minus the
 *          n-th number k such that 6k+1 is composite.
 * https://oeis.org/A173229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046953(n) – A046954(n) where A046953(n) = n‑th positive
 *            integer m with composite 6m−1, A046954(n) = n‑th positive
 *            integer m with composite 6m+1
 * Range:     u8 n <= 15, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n‑th term of OEIS A173229.  "seq_t" is the build‑specific
   integer type; "idx_t" is an unsigned 64‑bit integer.   */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* static memoisation state */
    static idx_t m_cnt   = 0;          /* number of stored m values      */
    static idx_t k_cnt   = 0;          /* number of stored k values      */
    static idx_t m_cap   = 0;          /* current capacity of m_arr      */
    static idx_t k_cap   = 0;          /* current capacity of k_arr      */
    static idx_t m_last  = 0;          /* last tested m candidate        */
    static idx_t k_last  = 0;          /* last tested k candidate        */
    static idx_t *m_arr  = NULL;       /* array of m values              */
    static idx_t *k_arr  = NULL;       /* array of k values              */

    /* enlarge arrays as required */
    if (m_cap < n) {
        size_t new_cap = (m_cap ? m_cap * 3 / 2 : 128);
        if (new_cap < n) new_cap = (size_t)n;
        idx_t *tmp = (idx_t *)realloc(m_arr, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        m_arr = tmp;
        m_cap = (idx_t)new_cap;
    }
    if (k_cap < n) {
        size_t new_cap = (k_cap ? k_cap * 3 / 2 : 128);
        if (new_cap < n) new_cap = (size_t)n;
        idx_t *tmp = (idx_t *)realloc(k_arr, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        k_arr = tmp;
        k_cap = (idx_t)new_cap;
    }

    /* generate m values: composites of 6m−1 */
    while (m_cnt < n) {
        ++m_last;
        seq_t cand = 6 * (seq_t)m_last - 1;
        if (!is_prime_big(cand))
            m_arr[m_cnt++] = m_last;
    }

    /* generate k values: composites of 6k+1 */
    while (k_cnt < n) {
        ++k_last;
        seq_t cand = 6 * (seq_t)k_last + 1;
        if (!is_prime_big(cand))
            k_arr[k_cnt++] = k_last;
    }

    /* compute difference */
    idx_t diff = m_arr[n - 1] - k_arr[n - 1];
    return (seq_t)diff;
}
