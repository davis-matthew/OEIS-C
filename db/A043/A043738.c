/*
 * A043738  Numbers whose number of runs in the base-2 representation is
 *          congruent to 5 mod 6.
 * https://oeis.org/A043738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th integer whose number of runs in the binary representation
   is congruent to 5 modulo 6 (OEIS A043738).  The implementation performs a
   simple brute–force search, testing each integer until the required term
   is found.  The test for the run condition uses only integer arithmetic
   and works for every build of seq_t. */
#include <stdint.h>
#include <stdbool.h>

static inline bool runs_congruent_five(uint64_t x)
{
    /* Find index of the most‑significant bit.  For x >= 1 this loop
       executes at most 64 times. */
    unsigned int msb = 63;
    while (((x >> msb) & 1u) == 0u)
        --msb;

    unsigned int prev_bit = (x >> msb) & 1u;
    unsigned int trans_mod6 = 0u;

    for (int i = (int)msb - 1; i >= 0; --i) {
        unsigned int cur_bit = (x >> i) & 1u;
        if (cur_bit != prev_bit) {
            trans_mod6 = (trans_mod6 + 1u) % 6u;   /* transition count mod 6 */
            prev_bit = cur_bit;
        }
    }

    /* Number of runs = transitions + 1.  We need runs % 6 == 5, i.e.
       transitions % 6 == 4. */
    return trans_mod6 == 4u;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the sequence, but safe */

    uint64_t cnt = 0;
    uint64_t x = 1;

    while (cnt < (uint64_t)n) {
        if (runs_congruent_five(x)) {
            ++cnt;
            if (cnt == (uint64_t)n)
                return (seq_t)x;
        }
        ++x;
    }

    /* We should never reach here for valid n. */
    return (seq_t)0;
}
