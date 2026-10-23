/*
 * A025400  Numbers that are the sum of 3 distinct positive cubes in exactly 2
 *          ways.
 * https://oeis.org/A025400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 572, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th integer that can be expressed as the sum of
   three distinct positive cubes in exactly two distinct ways.
   The routine enumerates all sums of three distinct cubes up to a
   preset maximum (20 000 000), counts the occurrences, stores the
   numbers that appear exactly twice, and returns the n‑th in that
   sorted list.  The algorithm is O(M^(3/2)) with M = 20 000 000. */

/* Include only C headers acceptable for both C11 and C++17. */
#include <stdint.h>
#include <stdlib.h>

/* Static data used for memoisation. */
static seq_t *a_list   = NULL; /* pointers to the terms found */
static size_t a_len    = 0;    /* number of terms stored */
static int    a_done   = 0;    /* flag indicating that a_list is ready */

/* Initialise the table of terms the first time a() is called. */
static void init_sequence(void)
{
    /* Maximum sum to check. */
    const size_t MAX_SUM = 20000000U;          /* 20 M is enough for 10 k terms */

    /* Allocate an array to count representations of each sum. */
    uint8_t *cnt = (uint8_t*)calloc(MAX_SUM + 1, sizeof(uint8_t));
    if (!cnt) abort();                         /* insufficient memory */

    /* Pre‑compute cubes up to cube_root(MAX_SUM). */
    size_t max_a = 0;
    /* Use a dynamic array of uint64_t for cube values; capacity 1024 is ample. */
    uint64_t cubes[1024];
    for (uint64_t a = 1; ; ++a) {
        uint64_t v = a * a * a;
        if (v > MAX_SUM) break;
        cubes[(size_t)a] = v;      /* store at index a (1‑based) */
        max_a = (size_t)a;
    }

    /* Count representations: sums a<b<c. */
    for (size_t a = 1; a <= max_a; ++a) {
        uint64_t ca = cubes[a];
        for (size_t b = a + 1; b <= max_a; ++b) {
            uint64_t cb = cubes[b];
            uint64_t sum_ab = ca + cb;
            /* Smallest possible third cube is cubes[b+1]. */
            if (b == max_a) continue;
            if (sum_ab + cubes[b + 1] > MAX_SUM) break;
            for (size_t c = b + 1; c <= max_a; ++c) {
                uint64_t s = sum_ab + cubes[c];
                if (s > MAX_SUM) break;
                ++cnt[(size_t)s];
            }
        }
    }

    /* Collect sums that appear exactly twice. */
    size_t cap = 128;
    seq_t *list = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!list) abort();
    size_t len = 0;

    for (size_t s = 1; s <= MAX_SUM; ++s) {
        if (cnt[s] == 2) {
            if (len == cap) {  /* grow capacity */
                cap <<= 1;      /* double */
                seq_t *tmp = (seq_t*)realloc(list, cap * sizeof(seq_t));
                if (!tmp) abort();
                list = tmp;
            }
            list[len++] = (seq_t)s;
        }
    }

    /* Clean up. */
    free(cnt);

    /* Store the result for future calls. */
    a_list = list;
    a_len  = len;
    a_done = 1;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (!a_done) init_sequence();
    if (n == 0 || (size_t)n > a_len) return 0; /* undefined for out of range */
    return a_list[(size_t)n - 1];
}
