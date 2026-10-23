/*
 * A029859  Euler transform of 3 2 1 1 1 1 1 1...
 * https://oeis.org/A029859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = (1/n) * Σ_{k=1}^{n} s(k) * a(n-k),
 *            where s(k) = Σ_{d|k} d * b(d-1), b(0)=3, b(1)=2, b(k)=1 for k≥2.
 *            This simplifies to s(k) = σ(k) + 2 if k is odd, s(k) = σ(k) + 4
 *            if k is even, with σ(k) the sum of divisors of k
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif
#include <stdlib.h>

/* ------------- globals ------------- */
static seq_t *a_cache = NULL;          /* a(n) values    */
static seq_t *s_cache = NULL;          /* s(k) values    */
static idx_t cache_size = 0;           /* size of caches */

/* ------------- helper: rebuild up to n ------------- */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* already computed */

    idx_t new_size = n + 1;
    a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    s_cache = (seq_t *)realloc(s_cache, new_size * sizeof(seq_t));

    /* compute divisor sum σ(k) for 0 ≤ k < new_size   */
    uint64_t *sigma = (uint64_t *)malloc(new_size * sizeof(uint64_t));
    for (idx_t i = 0; i < new_size; ++i) sigma[i] = 0;
    for (idx_t d = 1; d < new_size; ++d) {
        for (idx_t j = d; j < new_size; j += d)
            sigma[j] += d;
    }

    /* compute s(k) = σ(k)+2 (odd) or σ(k)+4 (even)      */
    for (idx_t i = 0; i < new_size; ++i) {
        if (i == 0)
            s_cache[i] = 0;                    /* unused */
        else
            s_cache[i] = (seq_t)(sigma[i] + ((i & 1ULL) == 0 ? 4ULL : 2ULL));
    }
    free(sigma);

    /* initial term a(0) */
    if (cache_size == 0) a_cache[0] = 1;

    /* compute a(n) for new indices */
    for (idx_t i = (cache_size == 0 ? 1 : cache_size); i < new_size; ++i) {
#ifdef __cplusplus
        /* For integral seq_t (≤ 8 bytes) use 128‑bit intermediates */
        if constexpr (std::is_unsigned<seq_t>::value && sizeof(seq_t) <= 8) {
            unsigned __int128 temp = 0;
            for (idx_t k = 1; k <= i; ++k)
                temp += (unsigned __int128)s_cache[k] *
                        (unsigned __int128)a_cache[i - k];
            a_cache[i] = (seq_t)(temp / i);
        } else {                                 /* arbitrary‑precision */
            seq_t temp = 0;
            for (idx_t k = 1; k <= i; ++k)
                temp += s_cache[k] * a_cache[i - k];
            a_cache[i] = temp / (seq_t)i;
        }
#else
        /* Pure C: seq_t is assumed to be an unsigned integer type. */
        unsigned __int128 temp = 0;
        for (idx_t k = 1; k <= i; ++k)
            temp += (unsigned __int128)s_cache[k] *
                    (unsigned __int128)a_cache[i - k];
        a_cache[i] = (seq_t)(temp / i);
#endif
    }
    cache_size = new_size;
}

/* ------------- required public function ------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
