/*
 * A239222  Cubes that are divisible by each of their nonzero digits.
 * https://oeis.org/A239222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = [m^(n)]^3 where m^(n) is the nth member of OEIS A239221:
 *            integers whose cube is divisible by each of their non-zero
 *            decimal digits
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 98, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

static seq_t *cubes = NULL;           /* array of found cubes  */
static size_t nfound = 0;             /* how many stored          */
static size_t capacity = 0;           /* current allocated size   */
static idx_t last_root = 0;           /* largest root checked so far */

/* Ensure the cubes array can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (capacity >= need) return;
    size_t newcap = capacity ? capacity * 2 : 64;
    while (newcap < need) newcap <<= 1;
    seq_t *newp = (seq_t *)realloc(cubes, newcap * sizeof(seq_t));
    if (!newp) abort();
    cubes = newp;
    capacity = newcap;
}

/* Test whether a given cube satisfies the digit-divisibility
   property.  The cube is supplied as seq_t. */
static bool cube_valid(seq_t cube)
{
    unsigned char buf[64];
    size_t dlen;
    get_digits(cube, buf, &dlen); /* digits in little-endian */
    for (size_t i = 0; i < dlen; ++i) {
        unsigned char d = buf[i];
        if (d == 0) continue;
        seq_t rem = cube % (seq_t)d;
        if (rem != 0) return false;
    }
    return true;
}

/* Find and store cubes until we have at least n elements. */
static void extend_to(idx_t n)
{
    while (nfound < (size_t)n) {
        idx_t m = ++last_root;          /* next candidate root   */
        seq_t m_seq = (seq_t)m;
        seq_t cube = m_seq * m_seq * m_seq; /* compute m^3 safely */
        if (cube_valid(cube)) {
            ensure_capacity(nfound + 1);
            cubes[nfound++] = cube;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0 */
    extend_to(n);
    return cubes[n - 1];
}
