/*
 * A160470  Third right hand column of triangles A094665 and A083061.
 * https://oeis.org/A160470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = (10*n^3+117*n^2+455*n+588)*a(n)/(5*n^2+21*n+16) with
 *            a(0) = 4. The function uses a memoized recurrence
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
# include <type_traits>
#endif

/* compute the next term of the recurrence for index n (current term is a(n)) */
static inline seq_t next_value(idx_t n, seq_t prev) {
#if defined(__cplusplus)
    if constexpr (std::is_integral_v<seq_t>) {
        /* all fixed‑size integer types use a 128‑bit intermediate to avoid overflow */
        unsigned __int128 coeff = (unsigned __int128)10 * n * n * n
                               + (unsigned __int128)117 * n * n
                               + (unsigned __int128)455 * n + 588;
        unsigned __int128 den   = (unsigned __int128)5 * n * n
                               + (unsigned __int128)21 * n + 16;
        unsigned __int128 num   = coeff * (unsigned __int128)prev;
        return (seq_t)(num / den);
    } else {
        /* arbitrary‑precision types: rely on overloaded operators */
        seq_t nseq   = (seq_t)n;
        seq_t coeff  = (seq_t)10 * nseq * nseq * nseq
                    + (seq_t)117 * nseq * nseq
                    + (seq_t)455 * nseq + (seq_t)588;
        seq_t den    = (seq_t)5 * nseq * nseq
                    + (seq_t)21 * nseq + (seq_t)16;
        return coeff * prev / den;
    }
#else
    /* C implementation – 128‑bit intermediates when available */
    unsigned __int128 coeff = (unsigned __int128)10 * n * n * n
                            + (unsigned __int128)117 * n * n
                            + (unsigned __int128)455 * n + 588;
    unsigned __int128 den   = (unsigned __int128)5 * n * n
                            + (unsigned __int128)21 * n + 16;
    unsigned __int128 num   = coeff * (unsigned __int128)prev;
    return (seq_t)(num / den);
#endif
}

/* memoised recurrence for the sequence */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;   /* highest index for which cache[i] is valid */

    /* ensure enough space */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure – unlikely */
        cache = tmp;
        cap = new_cap;
    }

    /* initialise term a(0) if it hasn't been stored yet */
    if (max_computed == 0 && cap > 0) {
        cache[0] = (seq_t)4;
        max_computed = 0;
    }

    /* compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = next_value(i - 1, cache[i - 1]);
    }
    max_computed = n;
    return cache[n];
}
