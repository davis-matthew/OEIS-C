/*
 * A028812  Nonsquares mod 99.
 * https://oeis.org/A028812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-squares modulo 99:
   a(n) = the n-th positive integer that is not a quadratic residue modulo 99
   (i.e., not a square modulo 9 nor modulo 11).  The pattern repeats every
   99 integers; among each block of 99, exactly 75 numbers are non‑quadratic
   residues. */

/* Required headers */
#include <stdbool.h>
#include <stddef.h>

/* Static table of the 75 non‑residues in 1..98 inclusive (ordered). */
static seq_t NONRES[75];
static bool nonres_init = false;

/* Build the table once. */
static void init_nonres(void) {
    if (nonres_init) return;
    bool isres[99] = {0};
    for (idx_t t = 0; t < 99; ++t) {
        seq_t sq = (seq_t)t * t % (seq_t)99;      /* t^2 mod 99 */
        isres[(size_t)sq] = true;
    }
    size_t pos = 0;
    for (int i = 1; i < 99; ++i) {                  /* 1..98 (99 is 0 mod 99) */
        if (!isres[i]) {
            NONRES[pos++] = i;
        }
    }
    nonres_init = true;
}

/* OEIS A028812: nonsquares modulo 99 */
seq_t a(idx_t n) {
    /* Ensure the non‑residue table is ready. */
    if (!nonres_init) init_nonres();

    /* There are 75 non‑residues in each 99‑block. */
    idx_t block = (n - 1) / 75;    /* 0‑based block index   */
    idx_t in_block = (n - 1) % 75; /* position within the block */

    /* Compute the value: block*99 + NONRES[in_block] */
    seq_t val = (seq_t)block * 99 + NONRES[in_block];
    return val;
}
