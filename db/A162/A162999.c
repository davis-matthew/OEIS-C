/*
 * A162999  Positions of primes in A119509.
 * https://oeis.org/A162999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   _has_unique_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = position of the n‑th prime number in the list of integers whose square
   contains no repeated digit (OEIS A119509).  The sequence is finite: there are
   exactly 87 primes in A119509, the last one occurring at position 481. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_has_unique_digits.h"

static size_t _prime_positions_cap = 128;         // > 87
static seq_t *_prime_positions = NULL;            // positions of primes
static size_t _prime_positions_len = 0;           // number of positions stored

/* Very small prime test, suitable for numbers fitting into idx_t (≤ ~1e6). */
static bool _is_prime_small(idx_t n)
{
    if (n < 2)
        return false;
    if ((n & 1) == 0)
        return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2) {
        if ((n % d) == 0)
            return false;
    }
    return true;
}

/* Build the positions of the 87 primes in A119509.  Results are cached
   in the static array _prime_positions.  After the first call this
   work is not repeated. */
static void _build_prime_positions(void)
{
    if (_prime_positions != NULL)
        return;                                 // already built

    _prime_positions = (seq_t *)malloc(_prime_positions_cap * sizeof(seq_t));
    if (_prime_positions == NULL)
        return;                                 // allocation failed

    idx_t candidate = 1;        /* integer whose square we test */
    idx_t pos_in_seq = 0;       /* position in A119509 (1‑based) */

    while (_prime_positions_len < 87) {     /* we know there are exactly 87 */
        uint64_t sq = ((uint64_t)candidate) * ((uint64_t)candidate);
        if (_has_unique_digits(sq)) {
            ++pos_in_seq;                  /* candidate qualifies */
            if (_is_prime_small(candidate)) {
                /* store position */
                if (_prime_positions_len >= _prime_positions_cap) {
                    /* enlarge buffer if needed (unlikely) */
                    _prime_positions_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(_prime_positions,
                                                   _prime_positions_cap *
                                                   sizeof(seq_t));
                    if (tmp != NULL)
                        _prime_positions = tmp;
                }
                _prime_positions[_prime_positions_len] = (seq_t)pos_in_seq;
                ++_prime_positions_len;
            }
        }
        ++candidate;
    }
}

/* -------------------------------------------------------------------------- */
/* External function required by the build framework.                           */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                     /* OEIS offset is 1, so a(0) unused */
        return 0;

    _build_prime_positions();

    if (n > (idx_t)_prime_positions_len)
        return 0;                    /* undefined for n beyond the known range */

    return _prime_positions[n - 1];
}
