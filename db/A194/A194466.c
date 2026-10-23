/*
 * A194466  Numbers m such that Sum_{k=1..m} (<c + k*r> - <k*r>) > 0, where
 *          r=sqrt(2) and c=sqrt(1/2), and < > denotes fractional part.
 * https://oeis.org/A194466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers m such that Σ_{k=1..m} ( {c + k r} – {k r} ) > 0, where
 *            r = √2 and c = √(1/2). The test reduces to #{k≤m: {k r}<T} > m T
 *            with T = 1 – c = (2‑√2)/2. The array stores the found m’s in
 *            increasing order; a(n) returns the n‑th element
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- memory helper that works in C and C++ ---------- */
static void *alloc_seq(size_t n_elements, size_t elem_size)
{
#if defined(__cplusplus)
    return reinterpret_cast<seq_t *>(std::malloc(n_elements * elem_size));
#else
    return malloc(n_elements * elem_size);
#endif
}
static void *realloc_seq(void *ptr, size_t n_elements, size_t elem_size)
{
#if defined(__cplusplus)
    return reinterpret_cast<seq_t *>(std::realloc(ptr, n_elements * elem_size));
#else
    return realloc(ptr, n_elements * elem_size);
#endif
}

/* ---------- main routine ---------- */
seq_t a(idx_t n)
{
    /* Constants that can be evaluated once */
    static const long double SQRT2
        = 1.41421356237309504880168872420969807856967187537694L;
    static const long double T = (2.0L - SQRT2) / 2.0L;   /* 1‑c */

    /* Static state that survives between calls */
    static seq_t *terms = NULL;       /* stored m values   */
    static size_t terms_cap = 0;      /* allocated slots   */
    static size_t terms_len = 0;      /* number of values  */
    static idx_t  next_m   = 1;       /* next m to test    */
    static uint64_t count_N1 = 0;     /* #{k≤m‑1 : frac(k r)<T} */

    /* Ensure the table holds at least n entries */
    while (terms_len < (size_t)n) {
        idx_t m = next_m;

        /* Fractional part {m · √2}.  Use long double to avoid overflow. */
        long double v   = (long double)m * SQRT2;
        long double frac = v - floorl(v);

        /* Count how many the fractional parts are < T up to m‑1 */
        if (frac < T) count_N1++;

        /* Check the inequality count_N1 > m T  (both sides real numbers) */
        if ((long double)count_N1 > (long double)m * T) {
            /* Grow table if needed */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 16;
                seq_t *new_ptr = (seq_t *)realloc_seq(terms, new_cap, sizeof(seq_t));
                terms = new_ptr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)m;
        }
        next_m++;
    }
    return terms[n - 1];
}
