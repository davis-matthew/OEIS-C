/*
 * A145517  Cubes ending in a (different) cube.
 * https://oeis.org/A145517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_cube_root, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145517: Cubes ending in a (different) cube.
   The algorithm iterates over natural numbers m, computes m^3,
   and checks whether there exists a proper suffix of the decimal
   representation of m^3 that equals k^3 for some integer k ≠ m.
   The sequence is stored in a dynamic cache so that calls can be
   made in any order. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_cube_root.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Dynamic table of 10^i, used to extract suffixes of a cube. */
static seq_t *pow10 = NULL;
static size_t pow10_cap = 0;   /* allocated capacity of pow10 array   */
static size_t pow10_len = 0;   /* current number of valid entries     */

/* Extend the pow10 table by one element (10^pow10_len). */
static void extend_pow10(void)
{
    seq_t next = pow10[pow10_len - 1] * (seq_t)10;
    if (pow10_len == pow10_cap) {
        size_t new_cap = (pow10_cap == 0) ? 1 : pow10_cap * 2;
        seq_t *tmp = seq_realloc(pow10, pow10_cap, new_cap);
        pow10 = tmp;
        pow10_cap = new_cap;
    }
    pow10[pow10_len] = next;
    ++pow10_len;
}

/* Initialise the pow10 table once. */
static void init_pow10(void)
{
    if (pow10 != NULL)
        return;

    pow10 = seq_alloc(1);
    pow10[0] = (seq_t)1;
    pow10_cap = 1;
    pow10_len = 1;
}

/* Test whether cube = m^3 ends with the decimal digits of a different cube. */
static bool cube_has_suffix_cube(idx_t m)
{
    seq_t cube = (seq_t)m * (seq_t)m * (seq_t)m;

    /* Iterates over suffix lengths l = 1, 2, ... until 10^l > cube. */
    size_t l = 1;
    while (true) {
        if (l >= pow10_len)
            extend_pow10();

        if (pow10[l] > cube)
            break;

        seq_t suffix = cube % pow10[l];
        if (suffix == cube)
            continue;   /* full cube, not a proper suffix */

        seq_t root = floor_cube_root(suffix);
        if (root * root * root == suffix && root != (seq_t)m)
            return true;
        ++l;
    }
    return false;
}

/* Cache of sequence terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity of cache array  */
static size_t cache_size = 0;  /* number of terms currently stored   */
static idx_t next_m = 1;       /* next base to test for new terms    */

/* Ensure cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate sequence terms until at least 'n' terms are stored. */
static void generate_more(idx_t n)
{
    while ((idx_t)cache_size < n) {
        if (cache_size == 0) {
            /* The very first term is 0. */
            ensure_cache(1);
            cache[0] = (seq_t)0;
            cache_size = 1;
            next_m = 1;
            continue;
        }

        if (cube_has_suffix_cube(next_m)) {
            ensure_cache(cache_size + 1);
            seq_t cube = (seq_t)next_m * (seq_t)next_m * (seq_t)next_m;
            cache[cache_size++] = cube;
        }
        ++next_m;
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined for n < 1, but harmless */

    init_pow10();
    generate_more(n);
    return cache[(size_t)n - 1];
}
