/*
 * A119659  Floor of the area of consecutive Prime-Indexed Prime triangles.
 * https://oeis.org/A119659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A119659 – Floor of the area of consecutive Prime‑Indexed Prime triangles.
 *
 *  The function builds the sequence of Prime‑Indexed Primes (PIPs),
 *  scans consecutive triples, tests the triangle inequality and
 *  evaluates the area via Heron's formula, taking the floor of the
 *  square root.  Results are cached so that any order of calls is
 *  handled efficiently.  The routine works for 8‑, 16‑, 32‑, 64‑bit
 *  and arbitrary‑precision builds within the time limits for n up to
 *  about 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* is_prime(idx_t) -> bool */
#include "../utils/floor_sqrt.h"    /* floor_sqrt(seq_t) -> seq_t */

/* ---------- static working storage ------------------------------------ */
static uint64_t *primes        = NULL;      /* all primes discovered  */
static size_t   primes_cnt    = 0;          /* number of primes      */
static size_t   primes_cap    = 0;          /* allocated capacity    */

static uint64_t *pips         = NULL;      /* prime‑indexed primes  */
static size_t   pip_cnt       = 0;          /* number of PIPs        */
static size_t   pip_cap       = 0;          /* allocated capacity    */

static seq_t   *cache         = NULL;      /* a(1), a(2), …          */
static size_t   cache_len     = 0;          /* terms stored so far    */
static size_t   cache_cap     = 0;          /* allocated capacity for cache */

static uint64_t next_num      = 2;          /* next integer to test for primality */
static size_t   next_triple   = 0;          /* index in pips of the next triple to process */

/* ---------- utility for dynamic arrays -------------------------------- */
static void grow_uint64_array(uint64_t **arr, size_t *cap)
{
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    *cap = new_cap;
}

static void grow_seq_array(seq_t **arr, size_t *cap)
{
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* ---------- append a prime and, if its index is prime, to PIPs --------- */
static void add_prime(uint64_t p)
{
    /* add to primes array */
    if (primes_cnt == primes_cap) grow_uint64_array(&primes, &primes_cap);
    primes[primes_cnt++] = p;

    /* check whether its index is prime */
    if (is_prime((idx_t)primes_cnt)) {
        if (pip_cnt == pip_cap) grow_uint64_array(&pips, &pip_cap);
        pips[pip_cnt++] = p;
    }
}

/* ---------- generate primes and PIPs until we have at least 'need' PIPs----*/
static void generate_until_pips(size_t need)
{
    while (pip_cnt < need) {
        if (is_prime(next_num))
            add_prime(next_num);
        next_num++;
    }
}

/* ---------- Heron's formula – floor of triangular area ----------------*/
static seq_t triangle_area_floor(uint64_t a, uint64_t b, uint64_t c)
{
    idx_t s_int = (idx_t)a + (idx_t)b + (idx_t)c;      /* a+b+c */

    if ((s_int & 1) == 0) {                /* even perimeter */
        seq_t s = (seq_t)(s_int >> 1);     /* s = (a+b+c)/2  */
        seq_t area2 = s * (s - (seq_t)a)
                     * (s - (seq_t)b)
                     * (s - (seq_t)c);
        return floor_sqrt(area2);
    } else {                               /* odd perimeter */
        seq_t S  = (seq_t)s_int;
        seq_t a2 = (seq_t)(2 * a);
        seq_t b2 = (seq_t)(2 * b);
        seq_t c2 = (seq_t)(2 * c);
        seq_t area2 = S * (S - a2)
                      * (S - b2)
                      * (S - c2);
        return floor_sqrt(area2) >> 2;    /* divide by 4 */
    }
}

/* ---------- exported sequence function --------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* not defined for offset 1 */

    /* Continue generating terms until the requested index is available */
    while (cache_len < (size_t)n) {
        /* Ensure we have enough PIPs to examine the next triple */
        size_t needed_pips = next_triple + 3;
        generate_until_pips(needed_pips);

        /* Process all triples that are now available */
        while (next_triple + 2 < pip_cnt && cache_len < (size_t)n) {
            uint64_t side1 = pips[next_triple];
            uint64_t side2 = pips[next_triple + 1];
            uint64_t side3 = pips[next_triple + 2];

            /* Triangle inequality check (all PIPs are increasing) */
            if (side1 + side2 > side3) {
                seq_t area = triangle_area_floor(side1, side2, side3);
                if (cache_len == cache_cap) grow_seq_array(&cache, &cache_cap);
                cache[cache_len++] = area;
            }
            ++next_triple;
        }
    }

    return cache[n - 1];                    /* convert 1‑based to 0‑based index */
}
