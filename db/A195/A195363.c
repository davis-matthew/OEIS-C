/*
 * A195363  Six-digit exclusionary cubes: 6-digit numbers whose cubes comprise
 *          digits none of which appears in the number itself.
 * https://oeis.org/A195363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A195363 – Six‑digit exclusionary cubes
   (computed by searching all 6‑digit numbers 100000–999999
   and testing that none of the digits of n³ appear in n) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the framework; do not redeclare them. */

/* Cache of the 65 terms. */
static seq_t seq_cache[65];
static size_t seq_count = 0;
static bool seq_initialized = false;

/* Build the cache the first time an element is requested. */
static void init_sequence(void)
{
    if (seq_initialized)
        return;

    seq_count = 0;

    for (unsigned int x = 100000; x <= 999999; ++x) {
        /* Build a bit mask of the digits appearing in x. */
        unsigned int mask_x = 0;
        unsigned int tmp = x;
        while (tmp) {
            unsigned int d = tmp % 10;
            mask_x |= 1U << d;
            tmp   /= 10;
        }

        /* Compute x³ (fits in 64 bits). */
        uint64_t cube = (uint64_t)x * x * x;

        /* Test that none of the digits of cube appears in x. */
        bool ok = true;
        uint64_t y = cube;
        while (y) {
            unsigned int d = y % 10;
            if (mask_x & (1U << d)) {
                ok = false;
                break;
            }
            y /= 10;
        }

        if (ok) {
            seq_cache[seq_count++] = (seq_t)x;
            if (seq_count == 65)          /* all terms found */
                break;
        }
    }

    seq_initialized = true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    init_sequence();

    if (n < 1 || n > (idx_t)seq_count)
        return (seq_t)0;     /* out of bounds */

    return seq_cache[(size_t)(n - 1)];
}
