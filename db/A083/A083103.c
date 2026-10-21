/*
 * A083103  Second-order linear recurrence sequence with a(n) = a(n-1) +
 *          a(n-2), with initial terms 1786772701928802632268715130455793,
 *          1059683225053915111058165141686995.
 * https://oeis.org/A083103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 26, big any n
 * Helpers:   parse_seed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A083103
   Formula: a(n) = a(n-1) + a(n-2)
   Initial values: a(0) = 1786772701928802632268715130455793,
                   a(1) = 1059683225053915111058165141686995
   Computes the sequence for any n ≥ 0. */
#include <stdbool.h>
#include "../utils/parse_seed.h"

/* Global storage for the two seed values, lazily initialized. */
static seq_t g_seed0 = 0, g_seed1 = 0;
static bool g_init_done = false;

static void init_seeds(void)
{
    if (g_init_done) return;
    g_seed0 = parse_seed(
        "1786772701928802632268715130455793");
    g_seed1 = parse_seed(
        "1059683225053915111058165141686995");
    g_init_done = true;
}

/* Public sequence function.  Handles calls in any order. */
seq_t a(idx_t n)
{
    init_seeds();

    if (n == 0) return g_seed0;
    if (n == 1) return g_seed1;

    seq_t prev = g_seed0;
    seq_t cur  = g_seed1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + cur;
        prev = cur;
        cur  = next;
    }
    return cur;
}
