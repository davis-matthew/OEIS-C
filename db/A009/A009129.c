/*
 * A009129  Perimeter of more than one Pythagorean triangle.
 * https://oeis.org/A009129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 7393, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009129: perimeter of more than one Pythagorean triangle.
   The function enumerates all Pythagorean triples using Euclid's formula,
   counts for each perimeter how many triples produce it, and keeps only
   perimeters that occur at least twice.  The set of such perimeters is
   stored in a static array that is enlarged as needed until the requested
   index is available. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* --------- helpers ----------------------------------------------------- */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t r = a % b;
        a = b; b = r;
    }
    return a;
}

/* ----------- globals ---------------------------------------------------- */
static uint64_t _current_limit = 0;      /* largest perimeter processed     */
static seq_t * _valid_perims   = NULL;   /* sorted perimeters >=2 triples  */
static size_t _valid_len       = 0;      /* number of valid perimeters     */

/* free current state */
static void _free_global_state(void)
{
    if (_valid_perims != NULL) {
        free(_valid_perims);
        _valid_perims = NULL;
    }
    _valid_len    = 0;
    _current_limit = 0;
}

/* recompute all perimeters up to new_limit */
static void _recompute(uint64_t new_limit)
{
    /* counts of how many triples produce each perimeter */
    uint16_t *cnt = (uint16_t *)calloc((size_t)new_limit + 1, sizeof(uint16_t));
    if (!cnt) return;      /* unlikely, but keep code robust */

    /* m must satisfy 2*m*(m+n) <= new_limit  ->  m <= sqrt(new_limit/2) */
    uint64_t m_limit = (uint64_t)(sqrtl((long double)new_limit / 2.0L) + 1.0L);

    for (uint64_t m = 2; m <= m_limit; ++m) {
        for (uint64_t n = 1; n < m; ++n) {
            if (((m ^ n) & 1ULL) == 0ULL) continue;          /* opposite parity */
            if (gcd_uint64(m, n) != 1ULL) continue;         /* coprime */

            uint64_t p0 = 2ULL * m * (m + n);               /* primitive perimeter */
            if (p0 > new_limit) continue;

            for (uint64_t k = 1; k * p0 <= new_limit; ++k) {
                uint64_t p = k * p0;
                if (cnt[p] < 0xFFFFU) cnt[p]++;            /* avoid overflow   */
            }
        }
    }

    /* count how many perimeters have >=2 triples */
    size_t valid_count = 0;
    for (size_t i = 1; i <= new_limit; ++i)
        if (cnt[i] >= 2U) ++valid_count;

    /* allocate destination array */
    seq_t *new_list = NULL;
    if (valid_count > 0) {
        new_list = (seq_t *)calloc(valid_count, sizeof(seq_t));
        if (!new_list) {
            free(cnt);
            return;                                          /* allocation fail */
        }

        size_t idx = 0;
        for (size_t i = 1; i <= new_limit; ++i)
            if (cnt[i] >= 2U) new_list[idx++] = (seq_t)i;
    }

    /* replace old state */
    _free_global_state();
    _valid_perims = new_list;
    _valid_len    = valid_count;
    _current_limit = new_limit;

    free(cnt);
}

/* -------- public function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined index; harmless */

    if (_current_limit == 0) _recompute(2000);   /* start with a small limit */

    /* grow until we have at least n terms */
    while (_valid_len < (size_t)n) {
        uint64_t new_limit = _current_limit * 2ULL;
        if (new_limit == 0) new_limit = 2000ULL;   /* safety against overflow */
        _recompute(new_limit);
    }

    return _valid_perims[(size_t)n - 1];
}
