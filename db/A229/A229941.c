/*
 * A229941  Sequence of triples: the 10 solutions of 1/p + 1/q + 1/r = 1/2
 *          with 0 < p <= q <= r, lexicographically sorted.
 * https://oeis.org/A229941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, full, tabf
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A229941: enumerate all integer solutions to
   1/p + 1/q + 1/r = 1/2 with 0 < p <= q <= r, lexicographically
   sorted, and return the n‑th element of the flattened list. */

#include <stddef.h>
#include <stdbool.h>

static seq_t seq[30];
static bool initialized = false;

static void generate_sequence(void)
{
    idx_t idx = 0;
    for (seq_t p = 3; p <= 6; ++p)
    {
        for (seq_t q = p; ; ++q)
        {
            seq_t denom = p * q - 2 * q - 2 * p;
            if (denom <= 0)
                continue;
            seq_t numer = 2 * p * q;
            if (numer % denom != 0)
                continue;
            seq_t r = numer / denom;
            if (r < q)
                break;
            seq[idx++] = p;
            seq[idx++] = q;
            seq[idx++] = r;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized)
    {
        generate_sequence();
        initialized = true;
    }
    if (n == 0)
        return 0;
    idx_t pos = n - 1;
    if (pos >= 30)
        return 0;
    return seq[pos];
}
