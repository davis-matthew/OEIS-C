/*
 * A174230  a(n) = product_{i=1..n-1} a(i), with a(1) = 3 and a(2) = 7.
 * https://oeis.org/A174230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=1}^{n-1} a(i); a(1)=3, a(2)=7, a(3)=21; for
 *            n>=4, a(n)=a(n-1)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n<1 */

#ifdef __cplusplus
    /* C++ implementation using std::vector for arbitrary‐precision integer classes */
    static std::vector<seq_t> cache;
    if (cache.size() == 0) cache.push_back((seq_t)3);        /* a(1) */
    if (cache.size() == 1) cache.push_back((seq_t)7);        /* a(2) */
    if (cache.size() == 2) cache.push_back((seq_t)21);       /* a(3) */

    while (cache.size() < n) {
        seq_t const last = cache.back();
        cache.push_back(last * last);                       /* a(k)=a(k-1)^2 for k>=4 */
    }
    return cache[n - 1];
#else
    /* C implementation using malloc/realloc; seq_t must be a trivially
       constructible integer type (uint8_t … uint64_t, unsigned __int128). */
    static seq_t *cache = NULL;
    static idx_t len = 0;      /* number of elements stored */
    static idx_t cap = 0;      /* capacity of allocated block */

    if (len < n) {
        /* Grow capacity enough to hold at least n elements */
        idx_t new_cap = (cap == 0) ? 3 : cap * 2;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    if (len == 0) { cache[0] = (seq_t)3; len = 1; }           /* a(1) */
    if (len == 1) { cache[1] = (seq_t)7; len = 2; }           /* a(2) */
    if (len == 2) { cache[2] = (seq_t)21; len = 3; }          /* a(3) */

    while (len < n) {
        cache[len] = cache[len - 1] * cache[len - 1];         /* a(k)=a(k-1)^2 for k>=4 */
        ++len;
    }
    return cache[n - 1];
#endif
}
