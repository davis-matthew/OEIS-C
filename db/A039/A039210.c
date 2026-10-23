/*
 * A039210  Numbers whose base-12 representation has the same number of 0's
 *          and 1's.
 * https://oeis.org/A039210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-12 representation has the same number of 0's and 1's */
#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t last_tested = 1;      /* last number that was examined */
static size_t    term_cnt   = 0;      /* number of terms stored (1‑based) */

#ifdef __cplusplus
static std::vector<seq_t> cachev;     /* cache for C++ builds */
#else
static seq_t *cache   = NULL;         /* cache for C builds */
static size_t cachecap = 0;           /* allocated capacity of cache   */
#endif

/* Return true iff the base‑12 digit multiset contains the same
   number of 0’s and 1’s. */
static bool qualifies(uint64_t x)
{
    unsigned zeros = 0, ones = 0;
    do {
        uint8_t d = x % 12;
        if (d == 0)   zeros++;
        else if (d == 1) ones++;
        x /= 12;
    } while (x != 0);
    return zeros == ones;
}

/* Ensure the cache can hold indices up to 'need' (1‑based). */
static void ensure_cap(size_t need)
{
    if (need == 0) return;
#ifdef __cplusplus
    if (cachev.size() < need + 1)
        cachev.resize(need + 1);
#else
    if (cachecap < need + 1) {
        size_t newcap = cachecap ? cachecap * 2 : 16;
        while (newcap < need + 1) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cachecap = newcap;
    }
#endif
}

/* OEIS function a(n).  Indexing starts at 1. */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (term_cnt < target) {
        ensure_cap(target);
        while (term_cnt < target) {
            uint64_t x = ++last_tested;
            if (qualifies(x)) {
                term_cnt++;
#ifdef __cplusplus
                cachev[term_cnt] = (seq_t)x;
#else
                cache[term_cnt] = (seq_t)x;
#endif
            }
        }
    }
#ifdef __cplusplus
    return cachev[n];
#else
    return cache[n];
#endif
}
