/*
 * A118215  Start with 1 and repeatedly reverse the digits and add 68 to get
 *          the next term.
 * https://oeis.org/A118215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(1) = 1, a(n) = reverse_decimal(a(n-1)) + 68 (n>=2)
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* Decimal reversal: works for any seq_t that supports basic arithmetic */
static seq_t reverse_decimal(seq_t v)
{
    seq_t r = (seq_t)0;
    while (v != (seq_t)0) {
        r = r * (seq_t)10 + v % (seq_t)10;
        v = v / (seq_t)10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) {            /* behavior for n<1 is irrelevant */
        return (seq_t)0;
    }

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(32);
        cache.push_back((seq_t)1);     /* a(1) */
    }
    while ((idx_t)cache.size() < n) {
        seq_t prev = cache.back();
        seq_t rev  = reverse_decimal(prev);
        seq_t next = rev + (seq_t)68;
        cache.push_back(next);
    }
    return cache[(size_t)n - 1];
#else
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    if (len == 0) { /* initial allocation */
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* unlikely OOM */
        cache[0] = (seq_t)1;                 /* a(1) */
        len = 1;
    }

    while (len < (size_t)n) {
        seq_t prev = cache[len - 1];
        seq_t rev  = reverse_decimal(prev);
        seq_t next = rev + (seq_t)68;

        if (len >= cap) { /* grow cache */
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                free(cache);
                return (seq_t)0;
            }
            cache = tmp;
            cap = new_cap;
        }
        cache[len] = next;
        ++len;
    }
    return cache[(size_t)n - 1];
#endif
}
