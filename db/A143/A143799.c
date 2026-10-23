/*
 * A143799  Lesser of Twin prime numbers of the form : i^2+j^3, as sum of
 *          square and cube, if Greater Twin prime number also of the form :
 *          i^2+j^3, as sum of square and cube.
 * https://oeis.org/A143799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A143799:
   the smaller of each twin‑prime pair (p, p+2) with p and p+2 both of
   the form i² + j³ for positive integers i and j.  The function is
   memoised; calling order of n may be arbitrary. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t)      */
#include "../utils/floor_sqrt.h"    /* seq_t floor_sqrt(seq_t)  */

/* ------------------------------------------------------------------ */
/* Integer cube root: floor(cuberoot(x)) using only integer ops. */
static inline seq_t _floor_cbrt(seq_t x)
{
    seq_t low  = 0;
    seq_t high = floor_sqrt(x);          /* sqrt(x) >= cbrt(x)      */
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;
        seq_t mid_sq = mid * mid;
        seq_t mid_cu = mid_sq * mid;
        if (mid_cu <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Test whether x can be expressed as i² + j³ for some positive i, j. */
static bool _can_be_sum_of_square_and_cube(seq_t x)
{
    seq_t max_j = _floor_cbrt(x);
    for (idx_t j = 1; j <= (idx_t)max_j; ++j) {
        seq_t j_cu = (seq_t)j * (seq_t)j * (seq_t)j;
        seq_t rem  = x - j_cu;
        seq_t sq   = floor_sqrt(rem);
        if (sq * sq == rem)
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Cache for computed terms. */
static size_t    _cache_len = 0;
static size_t    _cache_cap = 0;
static seq_t   *_cache_data = NULL;

/* Next odd number to test for twin‑prime status. */
static idx_t    _next_candidate = 3;

/* ------------------------------------------------------------------ */
/* Grow cache storage. */
static void _grow_cache(void)
{
    if (_cache_len >= _cache_cap) {
        size_t new_cap = (_cache_cap == 0) ? 16 : _cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(_cache_data, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        _cache_data = tmp;
        _cache_cap  = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure cache contains at least n terms. */
static void _ensure_cache(idx_t n)
{
    while (_cache_len < n) {
        /* Search for the next twin‑prime pair meeting the criteria. */
        while (true) {
            seq_t candidate = (seq_t)_next_candidate;
            /* Skip even numbers – _next_candidate is always odd. */
            if (is_prime(_next_candidate) &&
                is_prime(_next_candidate + 2)) {

                seq_t p = candidate;
                seq_t q = candidate + 2;
                if (_can_be_sum_of_square_and_cube(p) &&
                    _can_be_sum_of_square_and_cube(q)) {

                    _grow_cache();
                    _cache_data[_cache_len++] = p;
                    /* Advance past this pair to avoid repetition. */
                    _next_candidate = _next_candidate + 2;
                    break;
                }
            }
            /* Move to the next odd candidate. */
            _next_candidate += 2;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public entry function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for n = 0 */

    _ensure_cache(n);
    return _cache_data[n - 1];
}
