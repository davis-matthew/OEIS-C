/*
 * A158931  a(n) = smallest number m such that m^2 and n^2 share no common
 *          digits and m^2 and n^2 together use all 10 digits, a(n) = 0 if no
 *          such m exists.
 * https://oeis.org/A158931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A158931
    a(n) = smallest m such that
           m^2 and n^2 share no common decimal digits,
           and together they use all ten digits 0‑9;
    a(n) = 0 if no such m exists.
    INDEXED FROM 1 (a(1) = 45136, a(2) = 36137, …).
*/

#include <stdint.h>
#include <stdbool.h>

/* cache for the minimal m for every possible digit mask of a square.
   mask bits 0..9 correspond to digits 0..9.  */
static seq_t best[1 << 10];
static bool has_best[1 << 10] = {0};
static bool precomputed = false;

/* pre‑compute minimal m for all 1024 masks (0..1023).                */
static void precompute(void)
{
    if (precomputed)
        return;

    const idx_t LIMIT = 20000000;          /* big enough to hit almost all masks */
    int found = 0;

    for (idx_t m = 1; m <= LIMIT && found < (1 << 10); ++m) {
        uint64_t sq = (uint64_t)m * (uint64_t)m;   /* works for all m ≤ LIMIT */
        uint16_t mask = 0;
        uint64_t t = sq;
        do {
            mask |= 1u << (t % 10);
            t /= 10;
        } while (t);

        if (!has_best[mask]) {
            best[mask] = (seq_t)m;
            has_best[mask] = true;
            ++found;
        }
    }

    precomputed = true;
}

/* main sequence function: computes a(n) as defined above.           */
seq_t a(idx_t n)
{
    precompute();

    /* digit mask of n^2 (n is uint64_t so n^2 fits in uint64_t for n ≤ ~1e9) */
    uint64_t sq_n = (uint64_t)n * (uint64_t)n;
    uint16_t mask_n = 0;
    uint64_t t = sq_n;
    do {
        mask_n |= 1u << (t % 10);
        t /= 10;
    } while (t);

    /* digits that m^2 must have: the complement of mask_n */
    uint16_t free_mask = (~mask_n) & ((1u << 10) - 1);

    return has_best[free_mask] ? best[free_mask] : (seq_t)0;
}
