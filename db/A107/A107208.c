/*
 * A107208  Primes of the form 4x^2 + 23y^2.
 * https://oeis.org/A107208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A107208 – Primes of the form 4·x² + 23·y²
 *
 *  The function builds a table of such primes by testing successive
 *  primes in increasing order and storing those that admit a representation.
 *  The table is grown as needed; the function can be called in any order.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t n); */
#include "../utils/int_sqrt.h"   /* idx_t int_sqrt(idx_t x); */

/* ---------- global static state ----------------------------------- */
static seq_t *s_values = NULL;     /* table of found primes */
static size_t s_cap   = 0;         /* current allocation size       */
static size_t s_sz    = 0;         /* number of stored primes       */
static uint64_t s_next = 2;        /* next integer to test          */

/* ---------- helper functions --------------------------------------- */
static bool has_representation(uint64_t p)
{
    const idx_t ymax = int_sqrt((idx_t)(p / 23));
    for (idx_t y = 0; y <= ymax; ++y) {
        unsigned __int128 prod = (unsigned __int128)23 * y * y;
        if (prod > p) break;
        uint64_t rem = p - (uint64_t)prod;
        if (rem & 3) continue;              /* rem % 4 != 0 */
        idx_t t   = (idx_t)(rem >> 2);       /* (p - 23y²) / 4 */
        idx_t sx  = int_sqrt(t);
        if ((uint64_t)sx * (uint64_t)sx == t)
            return true;
    }
    return false;
}

static void extend_until(idx_t n)
{
    if (n <= (idx_t)s_sz)
        return;

    /* grow array if required */
    if (s_sz + (size_t)n > s_cap) {
        size_t new_cap = s_cap ? s_cap : 64;
        while (s_sz + (size_t)n > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(s_values, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        s_values = tmp;
        s_cap    = new_cap;
    }

    /* search for additional primes */
    while (s_sz < (size_t)n) {
        if (s_next < 2) s_next = 2;
        if (is_prime(s_next) && has_representation(s_next))
            s_values[s_sz++] = (seq_t)s_next;
        ++s_next;
    }
}

/* ---------- public function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (s_values == NULL) {              /* first call – initialise */
        s_cap = 64;
        s_values = (seq_t *)malloc(s_cap * sizeof(seq_t));
        if (!s_values) abort();
    }

    extend_until(n);
    return s_values[n - 1];
}
