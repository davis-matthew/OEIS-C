/*
 * A047965  Coordination sequence T3 for Zeolite Code MSO.
 * https://oeis.org/A047965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) – 3*a(n-2) + 3*a(n-3) – a(n-4) + 2*a(n-7) –
 *            4*a(n-8) + 4*a(n-9) – 2*a(n-10) + a(n-13) – 3*a(n-14) +
 *            3*a(n-15) – 2*a(n-16) + a(n-17) for n ≥ 20; initial values
 *            a(0)…a(19) are given below
 * Range:     u8 n <= 10, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Initial 20 terms of the sequence. */
static const seq_t init[20] = {
    (seq_t)1,  (seq_t)4,  (seq_t)12, (seq_t)21,  (seq_t)32,
    (seq_t)51, (seq_t)80, (seq_t)110, (seq_t)132, (seq_t)164,
    (seq_t)212, (seq_t)263, (seq_t)304, (seq_t)343, (seq_t)406,
    (seq_t)480, (seq_t)542, (seq_t)598, (seq_t)668, (seq_t)759
};

/* Compute a value for n ≥ 20 using the linear recurrence. */
static inline seq_t compute_term(const seq_t *cache, idx_t i)
{
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 pos = 0;
        unsigned __int128 neg = 0;
        pos += (unsigned __int128)2 * (unsigned __int128)cache[i-1];
        pos += (unsigned __int128)3 * (unsigned __int128)cache[i-3];
        pos += (unsigned __int128)2 * (unsigned __int128)cache[i-7];
        pos += (unsigned __int128)4 * (unsigned __int128)cache[i-9];
        pos += (unsigned __int128)1 * (unsigned __int128)cache[i-13];
        pos += (unsigned __int128)3 * (unsigned __int128)cache[i-15];
        pos += (unsigned __int128)1 * (unsigned __int128)cache[i-17];

        neg += (unsigned __int128)3 * (unsigned __int128)cache[i-2];
        neg += (unsigned __int128)1 * (unsigned __int128)cache[i-4];
        neg += (unsigned __int128)4 * (unsigned __int128)cache[i-8];
        neg += (unsigned __int128)2 * (unsigned __int128)cache[i-10];
        neg += (unsigned __int128)3 * (unsigned __int128)cache[i-14];
        neg += (unsigned __int128)2 * (unsigned __int128)cache[i-16];
        return (seq_t)(pos - neg);
    } else {
        seq_t pos = 0, neg = 0;
        pos += ((seq_t)2) * cache[i-1];
        pos += ((seq_t)3) * cache[i-3];
        pos += ((seq_t)2) * cache[i-7];
        pos += ((seq_t)4) * cache[i-9];
        pos += ((seq_t)1) * cache[i-13];
        pos += ((seq_t)3) * cache[i-15];
        pos += ((seq_t)1) * cache[i-17];

        neg += ((seq_t)3) * cache[i-2];
        neg += ((seq_t)1) * cache[i-4];
        neg += ((seq_t)4) * cache[i-8];
        neg += ((seq_t)2) * cache[i-10];
        neg += ((seq_t)3) * cache[i-14];
        neg += ((seq_t)2) * cache[i-16];
        return pos - neg;
    }
#else
    unsigned __int128 pos = 0;
    unsigned __int128 neg = 0;
    pos += (unsigned __int128)2 * (unsigned __int128)cache[i-1];
    pos += (unsigned __int128)3 * (unsigned __int128)cache[i-3];
    pos += (unsigned __int128)2 * (unsigned __int128)cache[i-7];
    pos += (unsigned __int128)4 * (unsigned __int128)cache[i-9];
    pos += (unsigned __int128)1 * (unsigned __int128)cache[i-13];
    pos += (unsigned __int128)3 * (unsigned __int128)cache[i-15];
    pos += (unsigned __int128)1 * (unsigned __int128)cache[i-17];

    neg += (unsigned __int128)3 * (unsigned __int128)cache[i-2];
    neg += (unsigned __int128)1 * (unsigned __int128)cache[i-4];
    neg += (unsigned __int128)4 * (unsigned __int128)cache[i-8];
    neg += (unsigned __int128)2 * (unsigned __int128)cache[i-10];
    neg += (unsigned __int128)3 * (unsigned __int128)cache[i-14];
    neg += (unsigned __int128)2 * (unsigned __int128)cache[i-16];
    return (seq_t)(pos - neg);
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache_cap == 0) {
        cache_cap = 20;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = init[i];
    }

    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (size_t i = cache_cap; i <= n; ++i) {
            cache[i] = compute_term(cache, (idx_t)i);
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}
