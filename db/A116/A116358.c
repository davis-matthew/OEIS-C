/*
 * A116358  Numbers that cannot be written as sum of products of two
 *          successive primes.
 * https://oeis.org/A116358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = numbers that cannot be written as sum of products of two
 *            successive primes
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   _recompute_missing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_recompute_missing.h"

/* Cached data for successive calls */
static idx_t          _current_limit   = 0;
static seq_t *        _missing_vals    = NULL;
static size_t         _missing_count   = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n=0 never requested */

    /* If cache insufficient, extend it */
    if (_missing_count < (size_t)n) {
        idx_t limit = (_current_limit > 0) ? _current_limit : (n * 2 + 10);
        while (true) {
            seq_t *new_missing = NULL;
            size_t new_count   = 0;
            if (!_recompute_missing(limit, &new_missing, &new_count)) {
                /* on failure return 0; sequence not defined */
                if (_missing_vals) free(_missing_vals);
                _missing_vals   = NULL;
                _missing_count  = 0;
                _current_limit  = 0;
                return 0;
            }
            if (_missing_vals) free(_missing_vals);
            _missing_vals  = new_missing;
            _missing_count = new_count;
            _current_limit = limit;
            if (_missing_count >= (size_t)n) break;
            limit <<= 1;  /* double the limit and retry */
        }
    }

    return _missing_vals[n - 1];
}
