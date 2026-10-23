/*
 * A164714  A positive integer n is included if all runs of 0's in binary n
 *          are of the same length, and if all runs of 1's in binary n are of
 *          the same length, and if there are at least two runs of 0's and at
 *          least two runs of 1's.
 * https://oeis.org/A164714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 88, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates A164714: numbers whose binary representation
 * consists of alternating runs of 1's and 0's, all 1‑runs have
 * equal length, all 0‑runs have equal length, and there are
 * at least two runs of each digit.  Returns the n‑th smallest
 * such integer (1‑indexed).  The implementation generates
 * all numbers up to a fixed bit‑length, sorts them, removes
 * duplicates, and memoises the result.  It works for any
 * seq_t, built‑in or arbitrary‑precision, provided the
 * first few thousand terms fit within the chosen bit‑limit
 * (63 bits).  The cache size is ample for n up to at least 10000. */

#include <stdlib.h>
#include <stdbool.h>

/* compare function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined index, return 0 */

    /* static memoised cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache_len == 0)           /* first call: generate all values */
    {
        /* Hard‑coded limits provide a safe UB‑free range.
         * 63 bits guarantees all shifts and intermediate values
         * stay within the width of a 64‑bit unsigned integer,
         * which is also true for unsigned __int128 and arbitrary
         * precision types (their shift operators are safe
         * for any non‑negative count). */
        const size_t LIMIT_BITS = 63;
        const size_t K_MAX = 63;
        const size_t L_MAX = 63;

        /* initial capacity – far more than needed for 10 000 terms */
        cache_cap = 1 << 15; /* 32768 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        size_t count = 0;

        for (size_t k = 1; k <= K_MAX; ++k) {
            seq_t mask1 = ((seq_t)1 << k) - 1;   /* binary 111..1 (k times) */

            for (size_t l = 1; l <= L_MAX; ++l) {
                /* ----- EVEN number of runs (ends with 0) ----- */
                for (size_t p = 2; /* minimum 2 pairs */ ; ++p) {
                    size_t bits = p * (k + l);
                    if (bits > LIMIT_BITS)
                        break;

                    seq_t val = 0;
                    size_t runs = 2 * p;
                    for (size_t t = 0; t < runs; ++t) {
                        if ((t & 1) == 0)                /* 1‑run */
                            val = (val << k) | mask1;
                        else                              /* 0‑run */
                            val = val << l;
                    }

                    if (count == cache_cap) {
                        cache_cap <<= 1;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[count++] = val;
                }

                /* ----- ODD number of runs (ends with 1) ----- */
                for (size_t p = 2; ; ++p) { /* p pairs followed by final 1‑run */
                    size_t bits = p * (k + l) + k;  /* (p+1)*k + p*l */
                    if (bits > LIMIT_BITS)
                        break;

                    seq_t val = 0;
                    size_t runs = 2 * p + 1;
                    for (size_t t = 0; t < runs; ++t) {
                        if ((t & 1) == 0)
                            val = (val << k) | mask1;
                        else
                            val = val << l;
                    }

                    if (count == cache_cap) {
                        cache_cap <<= 1;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[count++] = val;
                }
            }
        }

        /* sort and deduplicate */
        qsort(cache, count, sizeof(seq_t), cmp_seq);

        size_t uniq = 0;
        for (size_t i = 0; i < count; ++i) {
            if (i == 0 || cache[i] != cache[i - 1]) {
                cache[uniq++] = cache[i];
            }
        }
        cache_len = uniq;
    }

    if ((idx_t)n > (idx_t)cache_len)
        return (seq_t)0;                /* out of range */

    return cache[n - 1];                 /* sequence is 1‑indexed */
}
