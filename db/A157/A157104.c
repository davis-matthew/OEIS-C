/*
 * A157104  Arithmetic derivative of cubefree numbers.
 * https://oeis.org/A157104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A004709(n)) – the arithmetic derivative of the
 *            n‑th cubefree natural number
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- helper: test if an integer is cube-free -------------------------------- */
static bool is_cubefree(uint64_t n)
{
    if (n <= 1) return true;
    uint64_t m = n;

    /* factor 2 separately */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            m /= 2;
            if (++e >= 3) return false;
        }
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                if (++e >= 3) return false;
            }
        }
    }
    /* remaining prime factor, if any, has exponent 1 */
    return true;
}

/* ---- helper: arithmetic derivative of a 64‑bit integer -------------------- */
static seq_t derivative_uint64(uint64_t m)
{
    if (m <= 1) return (seq_t)0;

    seq_t total = 0;
    uint64_t temp = m;

    /* factor 2 */
    if (temp % 2 == 0) {
        int e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        seq_t m_val = (seq_t)m;
        total += (m_val * e) / 2;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t m_val = (seq_t)m;
            total += (m_val * e) / p;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t m_val = (seq_t)m;
        total += m_val / temp;   /* exponent e = 1 */
    }

    return total;
}

/* ---- cached data for the sequence ---------------------------------------- */
static seq_t *cached_vals   = NULL;          /* a(n) values, 1‑based */
static uint64_t *cached_nuc = NULL;          /* nth cubefree number, 1‑based */
static size_t cached_len   = 0;               /* number of cached entries */
static size_t cached_cap   = 0;               /* allocated capacity */
static uint64_t next_candidate = 1;           /* next integer to test for cube‑free */

/* ensure at least n entries are cached */
static void ensure_cache(idx_t n)
{
    if (cached_len >= n) return;

    /* grow arrays if necessary */
    if (cached_cap < n) {
        size_t new_cap = (size_t)n;
        seq_t *new_vals = (seq_t *)realloc(cached_vals, new_cap * sizeof(seq_t));
        if (!new_vals) abort();
        cached_vals = new_vals;
        uint64_t *new_nums = (uint64_t *)realloc(cached_nuc, new_cap * sizeof(uint64_t));
        if (!new_nums) abort();
        cached_nuc = new_nums;
        cached_cap = new_cap;
    }

    /* fill new entries */
    for (size_t i = cached_len; i < (size_t)n; ++i) {
        while (!is_cubefree(next_candidate)) ++next_candidate;
        cached_nuc[i] = next_candidate;
        cached_vals[i] = derivative_uint64(next_candidate);
        ++next_candidate;
    }
    cached_len = n;
}

/* ---- public sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    ensure_cache(n);
    return cached_vals[(size_t)n - 1];
}
