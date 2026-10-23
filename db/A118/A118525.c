/*
 * A118525  Start with 1 and repeatedly reverse the digits and add 6 to get
 *          the next term.
 * https://oeis.org/A118525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, a(n) = reverse_decimal(a(n-1)) + 6
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence repeats with period 30 after the first term.      */

#include <stdbool.h>
#include "../utils/reverse_decimal.h"

static bool seq_initialized = false;
/* Storage for the first 31 terms (indices 1..31).  The array has 32
   elements so that index 31 is valid (index 0 is unused). */
static seq_t seq_cache[32];

/* Populate the first 31 terms once. */
static void seq_init(void)
{
    if (seq_initialized) return;

    seq_cache[1] = (seq_t)1;
    seq_t cur = seq_cache[1];

    for (idx_t i = 2; i <= 31; ++i) {
        seq_t rev = reverse_decimal(cur);        /* reverse current term */
        cur = rev + (seq_t)6;                    /* add 6 to get next term */
        seq_cache[i] = cur;
    }
    seq_initialized = true;
}

/* Retrieve the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    seq_init();

    /* The first term is handled directly. */
    if (n == 1) return seq_cache[1];

    /* For the pre‑computed range simply return the cached value. */
    if (n <= 31) return seq_cache[n];

    /* After the initial term the sequence has period 30. */
    idx_t idx = ((n - (idx_t)2) % (idx_t)30) + (idx_t)2;
    return seq_cache[idx];
}
