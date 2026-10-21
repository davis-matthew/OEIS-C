/*
 * A057897  Numbers which can be written as m^k-k, with m, k > 1.
 * https://oeis.org/A057897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer that can be written as m^k - k
 *            with m>1, k>1 (OEIS A057897)
 * Range:     u8 n <= 5, u16 n <= 337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine generates m^k - k sequentially, storing all distinct
   values in a dynamic array.  Whenever the array does not yet contain
   at least n terms it expands the generation limit, re‑computes all
   values up to that limit, sorts, deduplicates, and repeats until the
   required term is reachable. */
/* The implementation is fully type‑generic for the seq_t and idx_t
   definitions supplied by the build system. No external dependencies
   beyond the standard library are used. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparison routine for qsort – works for integral seq_t or a
   class that supports operator<. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

/* Push a value into a growing array.  The array pointer may be
   updated if reallocation takes place. */
static void push_value(seq_t **bufptr, size_t *size, size_t *cap, seq_t val)
{
    if (*size >= *cap) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *newbuf = (seq_t *)realloc(*bufptr, newcap * sizeof(seq_t));
        if (!newbuf) exit(EXIT_FAILURE);  /* out of memory */
        *bufptr = newbuf;
        *cap    = newcap;
    }
    (*bufptr)[(*size)++] = val;
}

/* Generate all values m^k - k that are <= limit and > *current_limit,
   appending them to *values.  *current_limit is updated to limit.  */
static void generate_up_to(idx_t *current_limit, idx_t limit,
                           seq_t **values, size_t *vs, size_t *vc)
{
    /* For each exponent k >= 2 */
    for (int k = 2; ; ++k) {
        /* Minimum possible value for this k: 2^k - k */
        unsigned __int128 pow2 = 1;
        for (int i = 0; i < k; ++i) pow2 <<= 1;
        if (pow2 - k > limit) break;          /* larger k gives only bigger minima */

        /* For each base m >= 2 */
        for (idx_t m = 2; ; ++m) {
            unsigned __int128 p = 1;
            for (int e = 0; e < k; ++e) {
                p *= m;
                if (p > (unsigned __int128)(limit + k)) break;
            }
            if (p > (unsigned __int128)(limit + k)) break;

            unsigned long long raw = (unsigned long long)(p - k);
            if (raw > limit) break;
            if (raw > *current_limit) {
                seq_t val = (seq_t)raw;
                push_value(values, vs, vc, val);
            }
        }
    }
    *current_limit = limit;
}

/* Public sequence function – returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Static memoised data */
    static seq_t *values = NULL;   /* array of distinct values */
    static size_t vs = 0;          /* current number of stored values */
    static size_t vc = 0;          /* array capacity */
    static idx_t current_limit = 0;/* highest value already generated */

    /* Ensure we have at least n terms */
    while (vs < (size_t)n) {
        /* Choose a new generation limit.  Double the previous limit
           or take n*10 if the former is too small.  Keep it increasing. */
        idx_t new_limit = (current_limit ? current_limit * 2 : 1000);
        if (new_limit < n * 10) new_limit = n * 10;
        if (new_limit <= current_limit) new_limit = current_limit + 1;

        generate_up_to(&current_limit, new_limit, &values, &vs, &vc);

        /* Sort and deduplicate the array. */
        if (vs > 1) {
            qsort(values, vs, sizeof(seq_t), cmp_seq);
            size_t uniq = 1;
            for (size_t i = 1; i < vs; ++i) {
                if (values[i] != values[uniq - 1])
                    values[uniq++] = values[i];
            }
            vs = uniq;
        }
    }

    return values[n - 1];
}
