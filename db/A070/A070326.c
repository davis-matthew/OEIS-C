/*
 * A070326  Upper triangular array giving for each (x,y) the minimum modulus m
 *          such that x^3+y^3 is not congruent to a cube (mod m).
 * https://oeis.org/A070326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by searching the minimal modulus m≥2 such that
   the sum x³ + y³ is not congruent to a cube modulo m,
   where (x,y) are the antidiagonal coordinates of n in the
   upper‑triangular array. */

#include <stdlib.h>
#include <stdint.h>

static unsigned char **cube_cache = NULL;
static idx_t cube_cache_cap = 0;

/* Ensure the cache array is large enough for index m */
static void ensure_cube_cache(idx_t m)
{
    if (cube_cache_cap <= m) {
        idx_t new_cap = m + 1;
        unsigned char **tmp = (unsigned char**)realloc(cube_cache, new_cap * sizeof(unsigned char*));
        if (!tmp) abort();
        for (idx_t i = cube_cache_cap; i < new_cap; ++i) tmp[i] = NULL;
        cube_cache = tmp;
        cube_cache_cap = new_cap;
    }
}

/* Return a pointer to a bitset of size m describing which residues
   are cubic modulo m. The bitset is allocated on first use. */
static unsigned char *get_cube_set(idx_t m)
{
    ensure_cube_cache(m);
    if (!cube_cache[m]) {
        unsigned char *arr = (unsigned char*)calloc(m, sizeof(unsigned char));
        if (!arr) abort();
        for (idx_t a = 0; a < m; ++a) {
            uint64_t aa = (uint64_t)a * a % m;
            uint64_t cube = (aa * a) % m;
            arr[(size_t)cube] = 1;
        }
        cube_cache[m] = arr;
    }
    return cube_cache[m];
}

/* Map natural number n (offset 1) to coordinates (x,y) in the
   upper‑triangular array using antidiagonal enumeration. */
static void antidiagonal_coords(idx_t n, idx_t *x, idx_t *y)
{
    idx_t t = 1;
    while (t * (t + 1) / 2 < n) ++t;
    *x = t;
    *y = n - t * (t - 1) / 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t x, y;
    antidiagonal_coords(n, &x, &y);

    /* Compute s0 = x³ + y³ as a 64‑bit integer.  x and y are small
       (≤ 150 for n ≤ 10 000), so the value fits easily in 64 bits. */
    uint64_t s0 = (uint64_t)x * x * x + (uint64_t)y * y * y;

    for (idx_t m = 2; ; ++m) {
        uint64_t r = s0 % m;
        unsigned char *cube = get_cube_set(m);
        if (!cube[(size_t)r]) {
            return (seq_t)m;
        }
    }
}
