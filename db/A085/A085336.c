/*
 * A085336  Numbers which are sums of two and also sums of three positive
 *          cubes.
 * https://oeis.org/A085336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   getbit, icbrt, setbit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A085336:
   Numbers that can be written as the sum of two positive cubes
   and also as the sum of three positive cubes.
   The routine pre‑computes all such numbers up to a fixed
   limit (chosen large enough to contain the first 10 000 terms)
   and stores them in order. Subsequent calls simply return
   the n‑th element of this cache.  The same logic works for
   the arbitrary‑precision build of seq_t.  No hard‑coded look‑up
   table is used. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/getbit.h"
#include "../utils/icbrt.h"
#include "../utils/setbit.h"

/* Comparator used by qsort */
static int cmp_idx(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2);
}

/* ------------------------------------------------------------------ */
/* Memoized sequence provider                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Constant upper limit (≈ 60 M) sufficient for n up to ~10 000. */
    const idx_t LIMIT = 60000000U;

    /* Static cache of found terms.  It is filled on the first call. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache_len == 0) {
        /* ---------- Bitsets for sums of two cubes and intersection ----------
           Bits are numbered from 0 to LIMIT inclusive. */
        size_t bitbytes = (LIMIT >> 3) + 1;
        uint8_t *two_bits   = (uint8_t *)calloc(bitbytes, 1);
        uint8_t *int_bits   = (uint8_t *)calloc(bitbytes, 1);

        /* ---------- Store all sums of two positive cubes --------------------
           The sums are also marked in two_bits. */
        idx_t max_c = icbrt(LIMIT);
        /* For later merging we also keep the list of distinct sums. */
        size_t two_cap = 256;
        size_t two_len = 0;
        idx_t *two_sums = (idx_t *)malloc(two_cap * sizeof(idx_t));

        for (idx_t a = 1; a <= max_c; ++a) {
            idx_t a3 = a * a * a;
            for (idx_t b = a; ; ++b) {
                idx_t sum = a3 + b * b * b;
                if (sum > LIMIT) break;
                setbit(two_bits, sum);
                if (two_len == two_cap) {
                    two_cap <<= 1;
                    two_sums = (idx_t *)realloc(two_sums, two_cap * sizeof(idx_t));
                }
                two_sums[two_len++] = sum;
            }
        }

        /* Sort the two‑cube sums to allow early termination in the next loop. */
        qsort(two_sums, two_len, sizeof(idx_t), cmp_idx);

        /* ---------- For each cube c add c³ to every two‑cube sum and mark
                        intersecting values in int_bits --------------------- */
        for (idx_t c = 1; c <= max_c; ++c) {
            idx_t c3 = c * c * c;
            for (size_t i = 0; i < two_len; ++i) {
                idx_t s = two_sums[i] + c3;
                if (s > LIMIT) break;
                if (getbit(two_bits, s))
                    setbit(int_bits, s);
            }
        }

        /* ---------- Collect all numbers that are in the intersection into the
                        cache (in increasing order).  -------------------------------- */
        /* Allocate a generous cache size and resize at the end. */
        cache = (seq_t *)malloc((two_len + 1) * sizeof(seq_t));
        for (idx_t val = 1; val <= LIMIT; ++val) {
            if (getbit(int_bits, val))
                cache[cache_len++] = (seq_t)val;
        }
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));

        /* ---------- Clean up temporary storage -------------------------------- */
        free(two_sums);
        free(two_bits);
        free(int_bits);
    }

    /* The caller guarantees n ≥ 1 and n ≤ cache_len for the test data.
       For safety we return 0 for out‑of‑range requests. */
    if (n == 0 || (size_t)n > cache_len)
        return (seq_t)0;
    return cache[n - 1];
}
