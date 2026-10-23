/*
 * A044252  Numbers k such that string 0,1 occurs in the base 9 representation
 *          of k but not of k-1.
 * https://oeis.org/A044252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k (n ≥ 1) whose base‑9 representation *
 *            contains the substring "01" while the base‑9 representation of
 *            k‑1 does not
 * Range:     u8 n <= 3, u16 n <= 889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <cstddef>
#include <stdbool.h>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif

/* ----------------- low‑level test ----------------- */
#if defined(__cplusplus)
static bool has01_seq_t(seq_t x)
{
    if (x < (seq_t)9) return false;          /* fewer than two digits */
    seq_t prev = 0;
    bool first = true;
    const seq_t nine = (seq_t)9;
    while (x > 0)
    {
        seq_t d = x % nine;
        if (!first && prev == (seq_t)1 && d == (seq_t)0) return true;
        prev = d;
        first = false;
        x /= nine;
    }
    return false;
}
#else
static int has01_seq_t(seq_t x)
{
    if (x < (seq_t)9) return 0;              /* fewer than two digits */
    seq_t prev = 0;
    int first = 1;
    const seq_t nine = (seq_t)9;
    while (x > 0)
    {
        seq_t d = x % nine;
        if (!first && prev == (seq_t)1 && d == (seq_t)0) return 1;
        prev = d;
        first = 0;
        x /= nine;
    }
    return 0;
}
#endif

/* ----------------- cache for C ───────────────────── */
#if defined(__cplusplus)
/* C++ implementation ------------------------------ */
static std::vector<seq_t> cache_vec;
static seq_t last_k_cpp = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* value is undefined for offset 0 */
    if (n <= cache_vec.size()) return cache_vec[n-1];

    seq_t k = last_k_cpp + 1;
    while (cache_vec.size() < (size_t)n)
    {
        if (has01_seq_t(k) && !has01_seq_t(k-1))
            cache_vec.push_back(k);
        ++k;
    }
    last_k_cpp = k - 1;
    return cache_vec[n-1];
}
#else
/* C implementation --------------------------------- */
static seq_t *cache_seq = NULL;
static size_t cache_count = 0;
static size_t cache_cap   = 0;
static seq_t last_k = 0;

static void ensure_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = (cache_cap > 0) ? (cache_cap << 1) : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache_seq, new_cap * sizeof(seq_t));
    cache_seq = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* value is undefined for offset 0 */
    if (n <= cache_count) return cache_seq[n-1];

    seq_t k = last_k + 1;
    while (cache_count < (size_t)n)
    {
        if (has01_seq_t(k) && !has01_seq_t(k-1))
        {
            ensure_cap(cache_count + 1);
            cache_seq[cache_count++] = k;
        }
        ++k;
    }
    last_k = k - 1;
    return cache_seq[n-1];
}
#endif
