/*
 * A091227  Inverse function of A014580: position in A014580 if the n-th
 *          GF(2)[X] polynomial is irreducible, 0 otherwise.
 * https://oeis.org/A091227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 98 (2026-11-03)
 * Helpers:   ilog2_u64_to_u32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A091227: the position of the n-th
   GF(2)[X] polynomial if it is irreducible, otherwise 0.  The ordering
   of polynomials is monic, by increasing degree; for a fixed degree
   the lower coefficients run in ascending binary order. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ilog2_u64_to_u32.h"
#include "../utils/mem.h"

/* Utility: remainder of f divided by g over GF(2). */
static inline uint64_t _poly_mod(uint64_t f, uint64_t g) {
    if (g == 0) return f;
    unsigned deg_g = ilog2_u64_to_u32(g);
    while (f != 0) {
        unsigned deg_f = ilog2_u64_to_u32(f);
        if (deg_f < deg_g) break;
        f ^= g << (deg_f - deg_g);
    }
    return f;
}

/* Test whether f (monic of degree deg) is irreducible over GF(2). */
static bool _is_irreducible(uint64_t f, unsigned deg) {
    if (deg == 0) return false;          /* 1 is not considered irreducible */
    if (deg == 1) return true;           /* linear polynomials are irreducible */

    /* Check all possible proper monic divisors of degree 1 .. deg/2. */
    for (unsigned d = 1; d <= deg / 2; ++d) {
        uint64_t limit = 1ULL << d;          /* number of polynomials of degree d */
        for (uint64_t sub = 0; sub < limit; ++sub) {
            uint64_t g = (1ULL << d) | sub;   /* monic of degree d */
            if (_poly_mod(f, g) == 0)          /* divides exactly */
                return false;                  /* reducible */
        }
    }
    return true;                               /* no divisor found */
}

/* Memoisation state. */
static idx_t    _last_computed = 0;          /* largest n already computed    */
static seq_t   *_a_vals        = NULL;        /* table of a(n) values          */
static size_t   _a_cap         = 0;           /* allocated capacity            */

/* Current enumerated polynomial. */
static idx_t    _cur_deg  = 0;                /* degree of current polynomial  */
static idx_t    _bits_used = 0;               /* lower bits of current poly    */

/* Counter of irreducible polynomials seen so far. */
static seq_t   _rank_ctr = 0;

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Ensure the array _a_vals can hold indices up to n (inclusive). */
static void _ensure_capacity(idx_t n) {
    if ((size_t)n + 1 <= _a_cap) return;
    size_t new_cap = _a_cap ? _a_cap * 2 : 128;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, _a_vals, new_cap);
    if (!tmp && new_cap > 0) abort();
    _a_vals = tmp;
    _a_cap  = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* grow array and compute values up to n if needed */
    _ensure_capacity(n);
    while (_last_computed < n) {
        ++_last_computed;

        /* Build the polynomial for the current enumeration index. */
        uint64_t poly = ((uint64_t)1 << _cur_deg) | _bits_used;

        /* Test for irreducibility. */
        unsigned deg = _cur_deg;
        if (_is_irreducible(poly, deg)) {
            ++_rank_ctr;
            _a_vals[_last_computed] = _rank_ctr;
        } else {
            _a_vals[_last_computed] = 0;
        }

        /* Advance to the next polynomial. */
        ++_bits_used;
        if (_bits_used == ((idx_t)1 << _cur_deg)) {
            _bits_used = 0;
            ++_cur_deg;
        }
    }

    return _a_vals[n];
}
