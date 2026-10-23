/*
 * A182040  Integers whose decimal representation consists of three distinct
 *          digits, one appearing once, one appearing twice, and one appearing
 *          three times.
 * https://oeis.org/A182040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   Enumerates all 6‑digit integers whose decimal representation
 *            contains exactly three distinct digits, one occurring once, one
 *            twice, and one three times (leading zero is forbidden)
 * Range:     u8 none, u16 none, u32 n <= 9501, u64 n <= 9501, u128 n <= 9501,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Static cache of all terms (38880 terms, < 40000).  The array is
   large enough for every possible value of n that the function must
   support.  No dynamic allocation is performed, so the code compiles
   under both C11 and C++17. */
static seq_t cache[40000];
static size_t cache_size = 0;
static bool cache_built = false;

/* Comparator used by qsort.  It works for both unsigned integer types
   and big‑integer classes that overload < and >. */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    return (*pa > *pb) - (*pa < *pb);
}

/* Precomputed combinations of 3 indices out of {0..5}. */
static const int comb3[20][3] = {
    {0,1,2},{0,1,3},{0,1,4},{0,1,5},
    {0,2,3},{0,2,4},{0,2,5},{0,3,4},
    {0,3,5},{0,4,5},{1,2,3},{1,2,4},
    {1,2,5},{1,3,4},{1,3,5},{1,4,5},
    {2,3,4},{2,3,5},{2,4,5},{3,4,5}
};

/* All 6 permutations of indices {0,1,2}.  The first index gets
   multiplicity 1, the second multiplicity 2, the third multiplicity 3. */
static const int perms[6][3] = {
    {0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}
};

/* Build the full list of terms and sort it.  The routine is called
   lazily on the first call to a().  Sorting guarantees the terms
   are in ascending order, so indexing is trivial. */
static void build_cache(void)
{
    /* Enumerate all triples of distinct digits 0..9. */
    for (int d0 = 0; d0 <= 9; ++d0) {
        for (int d1 = d0 + 1; d1 <= 9; ++d1) {
            for (int d2 = d1 + 1; d2 <= 9; ++d2) {
                int digits[3] = {d0, d1, d2};

                /* Enumerate all assignments of multiplicities {1,2,3}. */
                for (int pi = 0; pi < 6; ++pi) {
                    int mult[3];
                    for (int k = 0; k < 3; ++k)
                        mult[perms[pi][k]] = k + 1;

                    int idx3 = -1, idx2 = -1, idx1 = -1;
                    for (int k = 0; k < 3; ++k) {
                        if (mult[k] == 3) idx3 = k;
                        else if (mult[k] == 2) idx2 = k;
                        else if (mult[k] == 1) idx1 = k;
                    }

                    int digit3 = digits[idx3];
                    int digit2 = digits[idx2];
                    int digit1 = digits[idx1];

                    /* Iterate over all ways to place the three-occurring
                       digit in the 6 positions. */
                    for (int c3 = 0; c3 < 20; ++c3) {
                        int pos[6];
                        memset(pos, -1, sizeof(pos));
                        for (int t = 0; t < 3; ++t)
                            pos[comb3[c3][t]] = digit3;

                        /* Remaining positions (three of them). */
                        int rem[3], rcnt = 0;
                        for (int t = 0; t < 6; ++t)
                            if (pos[t] == -1) rem[rcnt++] = t;

                        /* Choose two of the remaining positions for the
                           two‑occurring digit. */
                        for (int i = 0; i < 3; ++i) {
                            for (int j = i + 1; j < 3; ++j) {
                                int pos2 = rem[i], pos3 = rem[j];
                                pos[pos2] = digit2;
                                pos[pos3] = digit2;

                                /* The third remaining position gets the
                                   one‑occurring digit. */
                                for (int k = 0; k < 3; ++k)
                                    if (k != i && k != j)
                                        pos[rem[k]] = digit1;

                                /* Skip numbers with leading zero. */
                                if (pos[0] == 0)
                                    continue;

                                /* Construct the number. */
                                seq_t val = 0;
                                for (int d = 0; d < 6; ++d)
                                    val = val * 10 + (seq_t)pos[d];

                                cache[cache_size++] = val;
                            }
                        }
                    }
                }
            }
        }
    }

    /* Sort the list in ascending order. */
    qsort(cache, cache_size, sizeof(seq_t), seq_cmp);
    cache_built = true;
}

/* Return the nth element of the sequence (a(1) = 100012). */
seq_t a(idx_t n)
{
    if (!cache_built)
        build_cache();

    /* In the OEIS specification the offset is 1.  If n is outside the
       valid range we return 0 – this value does not occur in the
       sequence, so it signals an out‑of‑range request. */
    if (n == 0 || n > (idx_t)cache_size)
        return 0;

    return cache[(size_t)(n - 1)];
}
