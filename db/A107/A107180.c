/*
 * A107180  Primes of the form 2x^2 + 35y^2.
 * https://oeis.org/A107180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 819, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 2x² + 35y²: computed by scanning integers, testing
   the congruence condition modulo 280 and primality. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

/* residues modulo 280 for which primes can occur */
static const uint16_t _residues[13] = {
    2, 37, 43, 53, 67, 93, 107, 123,
    163, 197, 253, 267, 277
};

static bool _ok_table[280];
static bool _ok_initialized = false;

/* initialise _ok_table once */
static void _init_ok_table(void)
{
    if (!_ok_initialized) {
        for (int i = 0; i < 280; ++i)
            _ok_table[i] = false;
        for (size_t i = 0; i < sizeof(_residues) / sizeof(_residues[0]); ++i)
            _ok_table[_residues[i]] = true;
        _ok_initialized = true;
    }
}

/* a(n) – nth prime of the form 2x^2 + 35y^2, with n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined by OEIS, but harmless */

    _init_ok_table();

    /* The first term is the prime 2. */
    if (n == 1)
        return (seq_t)2;

    uint64_t candidate = 3;          /* start at the first odd candidate */
    uint64_t count     = 1;          /* 2 has already been counted */

    while (count < n) {
        /* test congruence condition */
        size_t r = (size_t)(candidate % 280);
        if (_ok_table[r] && is_prime_uint64(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2;               /* only odd numbers need be tested */
    }

    /* Should never reach here for valid n. */
    return (seq_t)2;
}
