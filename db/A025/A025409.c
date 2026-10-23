/*
 * A025409  Numbers that are the sum of 4 distinct positive cubes in exactly 2
 *          ways.
 * https://oeis.org/A025409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 439, u32 n <= 439, u64 n <= 439, u128 n <= 439,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates all numbers that are the sum of
   four distinct positive cubes in exactly two ways.
   The algorithm enumerates all quadruples a<b<c<d (a,b,c,d≥1) with
   cube values up to 250³. It counts how many times each sum occurs
   and keeps those sums that occur exactly twice.  The resulting list
   is cached so that every call to a(idx_t n) is O(1).  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The cache that holds the sequence.  seq_cache[n] is a(n+1). */
static seq_t *seq_cache = NULL;
static idx_t  seq_len   = 0;

/* Builds the entire sequence once. */
static void build_sequence(void)
{
    /* Bound for cube indices.  250 gives >10000 terms and keeps the
       memory footprint modest (about 63 MB for the counter array). */
    const idx_t CUBE_BOUND = 250;
    const idx_t MAX_SUM = 4 * CUBE_BOUND * CUBE_BOUND * CUBE_BOUND;   /* 4*B³ */
    const size_t ARRAY_SIZE = (size_t)MAX_SUM + 1;

    /* Counter of how many quadruple representations a sum has (capped at 3). */
    uint8_t *cnt = (uint8_t *)calloc(ARRAY_SIZE, sizeof(uint8_t));
    if (!cnt) return;   /* allocation failure – leave cache empty */

    /* Pre‑compute the cubes using 64‑bit integers (values fit in 64 bits). */
    uint64_t cubes[CUBE_BOUND + 1];
    for (idx_t i = 1; i <= CUBE_BOUND; ++i)
        cubes[i] = (uint64_t)i * (uint64_t)i * (uint64_t)i;

    /* Enumerate all quadruples a<b<c<d and count sums. */
    for (idx_t a = 1; a <= CUBE_BOUND - 3; ++a) {
        uint64_t a3 = cubes[a];
        for (idx_t b = a + 1; b <= CUBE_BOUND - 2; ++b) {
            uint64_t sum_ab = a3 + cubes[b];
            for (idx_t c = b + 1; c <= CUBE_BOUND - 1; ++c) {
                uint64_t sum_abc = sum_ab + cubes[c];
                for (idx_t d = c + 1; d <= CUBE_BOUND; ++d) {
                    uint64_t sum = sum_abc + cubes[d];
                    size_t idx = (size_t)sum;
                    if (cnt[idx] < 3) ++cnt[idx];
                }
            }
        }
    }

    /* First pass: count how many sums have exactly two representations. */
    size_t term_cnt = 0;
    for (size_t s = 0; s < ARRAY_SIZE; ++s)
        if (cnt[s] == 2) ++term_cnt;

    /* If no terms found, abort. */
    if (term_cnt == 0) {
        free(cnt);
        return;
    }

    /* Allocate array for the discovered terms. */
    seq_t *res = (seq_t *)malloc(term_cnt * sizeof(seq_t));
    if (!res) { free(cnt); return; }

    /* Second pass: store the sums in ascending order. */
    size_t pos = 0;
    for (size_t s = 0; s < ARRAY_SIZE; ++s) {
        if (cnt[s] == 2) {
            res[pos++] = (seq_t)s;   /* cast to seq_t – fine for all values */
        }
    }

    /* Store the result in the static cache. */
    seq_cache = res;
    seq_len   = (idx_t)term_cnt;

    free(cnt);
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (seq_len == 0) build_sequence();          /* first call builds the cache */
    if (n == 0 || n > seq_len) return 0;        /* out-of‑range */
    return seq_cache[n - 1];
}
