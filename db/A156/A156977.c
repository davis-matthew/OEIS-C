/*
 * A156977  Numbers n such that n^2 contains every decimal digit exactly once.
 * https://oeis.org/A156977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) is the n‑th integer whose square contains every decimal
 *            digit 0–9 exactly once (i.e., n² is a 10‑digit pandigital
 *            number)
 * Range:     u8 none, u16 n <= 42, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   pandigital_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pandigital_square.h"

/* Preallocated storage for the 87 terms of the sequence. */
static seq_t terms[87];
static int term_cnt = 0;

/* One‑time initialisation routine that fills the `terms` array. */
static void init_terms(void)
{
    if (term_cnt != 0) return;          /* already initialised */

    /* Largest 10‑digit pandigital number. */
    const uint64_t max_pandigital = 9876543210ULL;
    for (uint64_t n = 1; n * n <= max_pandigital; ++n) {
        if (pandigital_square(n)) {
            terms[term_cnt++] = (seq_t)n;
            if (term_cnt == 87) break;
        }
    }
    /* For strictness, pad the remaining entries with zero. */
    while (term_cnt < 87) {
        terms[term_cnt++] = (seq_t)0;
    }
}

/* Public sequence function.  Index 1 corresponds to the first term. */
seq_t a(idx_t n)
{
    init_terms();
    if (n == 0) return (seq_t)0;
    if ((idx_t)(term_cnt) < n) return (seq_t)0;
    return terms[(size_t)n - 1];
}
