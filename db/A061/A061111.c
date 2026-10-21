/*
 * A061111  a(1) = 1; a(n) = smallest number such that the concatenation
 *          a(1).0^*.a(2).0^*....0^*.a(n) is a perfect cube (where any number
 *          of 0's can be inserted between the terms).
 * https://oeis.org/A061111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   floor_cube_root, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(1)=1.  For n>1, a(n) is the smallest positive integer
 * such that the concatenation a(1).a(2).…a(n) (with any number of
 * inserted zeros between the terms) forms a perfect cube.
 *
 * The algorithm keeps the current prefix cube C.  To obtain a(n+1),
 * it finds the smallest m≥1 for which there exists a cube K in the
 * interval [C·10^m , (C+1)·10^m).  The required a(n+1) is K - C·10^m,
 * and the new prefix cube becomes K.  This process is repeated to
 * generate the sequence.  Results are memoised so that calls to a(n)
 * may occur in any order.  The implementation works for primitive
 * integer seq_t as well as arbitrary‑precision types supported by the
 * build system.  It uses only integer arithmetic and the helper
 * functions floor_cube_root() and pow10() provided by the utils
 * library.  The static array size is 20000, sufficient for the
 * required n≈10000. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"
#include "../utils/pow10.h"

/* Maximum cache size.  Adjust if larger n values are required. */
#define MAX_CACHE 20000

/* Cached sequence values.
 * cache[i] holds a(i+1).  cache_len is the number of terms stored. */
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;

/* The cube of the current prefix (i.e. the value of the number formed
 * by a(1)..a(k) after term k has been appended).  It is initialised
 * to 1 (the cube of the first term). */
static seq_t prev_cube = 0;

/* Compute a(n+1) given the current prefix cube.  The result is
 * stored in *next_val, and the new prefix cube in *new_prefix. */
static void compute_next(seq_t prefix_cube, seq_t *next_val, seq_t *new_prefix) {
    seq_t pow10m;
    idx_t m = 1;
    while (1) {
        pow10m = pow10(m);                     /* 10^m */
        seq_t low  = prefix_cube * pow10m;      /* C·10^m */
        seq_t high = (prefix_cube + (seq_t)1) * pow10m - (seq_t)1;
        /* Bottom of the search interval */
        seq_t root  = floor_cube_root(low);
        root = root + (seq_t)1;                 /* First integer with root^3 > low */
        seq_t root2 = root * root;
        seq_t cube  = root2 * root;             /* root^3 */
        if (cube <= high) {                     /* Valid cube found */
            *next_val   = cube - low;           /* a(n+1) */
            *new_prefix = cube;                 /* New prefix cube */
            return;
        }
        /* No cube in this interval; try a longer block of zeros */
        m = m + (idx_t)1;
    }
}

/* Public API: return a(n) for n>=1.  Behaviour for n<1 is undefined
 * except that a(0) returns 0 for safety. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* arbitrary safe value */
    if (cache_len == 0) {  /* first time called */
        cache[0] = (seq_t)1;    /* a(1) = 1 */
        cache_len = 1;
        prev_cube = (seq_t)1;   /* cube after the first term */
    }
    if (n <= (idx_t)cache_len) {
        return cache[n - 1];
    }
    /* Extend cache up to n. */
    while (cache_len < (size_t)n) {
        seq_t next_val, new_prefix;
        compute_next(prev_cube, &next_val, &new_prefix);
        cache[cache_len] = next_val;
        cache_len++;
        prev_cube = new_prefix;
    }
    return cache[n - 1];
}
