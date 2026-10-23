/*
 * A179622  a(n) is the sum of previous numbers found using n's nonzero digits
 *          as offsets.
 * https://oeis.org/A179622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n < 10 a(n) = Σ a(n‑d) for each non‑zero digit d of
 *            n if n ≥ 10
 * Range:     u8 n <= 40, u16 n <= 102, u32 n <= 122, u64 n <= 180,
 *            u128 n <= 324, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- Common headers ---------- */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
/* In C++ use a std::vector for convenient dynamic allocation
   and automatic construction/destruction of seq_t objects  */
#include <vector>
#endif

/* ---------- The function ---------- */
seq_t a(idx_t n)
{
    /* Ensure the memoisation table can address index n.
       In both C and C++ we grow it as required.  */
    /* Base values for n < 10 are always 1, so we treat them
       uniformly with the rest of the algorithm.  */

#if defined(__cplusplus)  /* ----- C++ implementation ----- */
    /* Static std::vector memoisation table */
    static std::vector<seq_t> cache;

    /* Resize to accommodate index n.  New entries are
       default‑constructed (zero for numeric types). */
    if (n >= cache.size()) {
        cache.resize(n + 1);   /* all new entries are zero-initialised */
    }

    /* If already computed, return it.  Zero indicates
       “not yet computed” because every term is ≥ 1.  */
    if (cache[(size_t)n] != 0) {
        return cache[(size_t)n];
    }

#else  /* ----- C implementation ----- */
    /* Static array allocated by realloc */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure capacity */
    if (n >= (idx_t)cache_size) {
        size_t needed = (size_t)n + 1;
        size_t new_size = cache_size == 0 ? 64 : cache_size;
        while (new_size < needed) new_size <<= 1;

        /* realloc returns void*, cast appropriately in C++ as well */
#if defined(__cplusplus)
        seq_t *tmp = static_cast<seq_t*>(realloc(cache, new_size * sizeof(seq_t)));
#else
        seq_t *tmp = realloc(cache, new_size * sizeof(seq_t));
#endif
        if (!tmp) abort();           /* allocation failed */

        /* New elements are uninitialised; set them to zero (uncomputed). */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;

        cache = tmp;
        cache_size = new_size;
    }

    if (cache[(size_t)n] != 0) {
        return cache[(size_t)n];
    }
#endif

    /* ------------------- Compute a(n) ------------------- */

    /* For n < 10 the answer is 1.  Store and return it. */
    if (n < 10) {
        /* Record the value so future calls can use it. */
#if defined(__cplusplus)
        cache[(size_t)n] = 1;
#else
        cache[(size_t)n] = 1;
#endif
        return 1;
    }

    /* n ≥ 10: sum over non‑zero decimal digits of n. */
    seq_t sum = 0;
    idx_t x = n;
    while (x > 0) {
        idx_t d = x % 10;              /* current digit */
        if (d > 0) {
            sum += a(n - d);           /* recursion: n-d < n */
        }
        x /= 10;
    }

    /* Store the result for future use. */
#if defined(__cplusplus)
    cache[(size_t)n] = sum;
#else
    cache[(size_t)n] = sum;
#endif

    return sum;
}
