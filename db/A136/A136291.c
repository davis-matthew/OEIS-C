/*
 * A136291  Array read by rows: row n is a sequence of numbers k such that n^k
 *          does not contain the digit n.
 * https://oeis.org/A136291
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, fini, full, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   multiply_by
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A136291.
   a(n) = the k such that n^k does not contain the decimal digit n,
   with the sequence formed by concatenating the rows for n = 2,3,4,...
   For n ≥ 10 the condition is vacuous (n is not a single decimal digit),
   so the row consists of all non‑negative integers 0,1,2 … */

/* All required headers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/multiply_by.h"

/* Tunable limits – enough to capture all known finite rows (bases 2–9) */
#define MAX_BASE        9          /* largest base that has a finite row   */
#define MAX_EXPO_LIMIT  2000       /* exponent limit for brute force search */
#define MAX_DIGITS      20000      /* buffer for decimal digits of base^k */

/* Static storage for the pre‑computed rows of bases 2..9 */
typedef idx_t idx_t;          /* external type, alias for convenience */
static size_t row_len[MAX_BASE + 1] = {0};     /* number of exponents per base   */
static size_t row_cap[MAX_BASE + 1] = {0};     /* allocated capacity per base    */
static uint32_t *row_data[MAX_BASE + 1] = {NULL}; /* arrays of exponents          */
static size_t total_finite = 0;     /* total number of terms in rows 2..9 */
static bool initialized = false;    /* flag to run once */

/* Pre‑compute rows for bases 2..9.  This is run lazily on the first call
   to a(). */
static void precompute(void) {
    if (initialized) return;

    uint8_t digits[MAX_DIGITS];
    size_t len;

    for (int base = 2; base <= MAX_BASE; ++base) {
        /* initialise storage for this base */
        row_cap[base] = 64;
        row_len[base] = 0;
        row_data[base] = (uint32_t *)malloc(row_cap[base] * sizeof(uint32_t));

        /* start with 1 = base^0 */
        len = 1;
        digits[0] = 1;

        for (idx_t k = 0; k <= MAX_EXPO_LIMIT; ++k) {
            /* check whether current power contains the digit 'base' */
            bool contains = false;
            for (size_t i = 0; i < len; ++i) {
                if (digits[i] == (uint8_t)base) { contains = true; break; }
            }
            if (!contains) {                /* store the exponent k */
                if (row_len[base] == row_cap[base]) {
                    row_cap[base] <<= 1;     /* double the capacity */
                    row_data[base] = (uint32_t *)realloc(row_data[base],
                                                row_cap[base] * sizeof(uint32_t));
                }
                row_data[base][row_len[base]++] = (uint32_t)k;
            }

            /* prepare the next power: base^(k+1) */
            multiply_by(digits, &len, (uint8_t)base);
        }
    }

    /* compute cumulative length of all finite rows (bases 2..9) */
    total_finite = 0;
    for (int base = 2; base <= MAX_BASE; ++base) total_finite += row_len[base];

    initialized = true;
}

/* Public entry point for the sequence */
seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;          /* undefined below offset – return 0 */

    precompute();                         /* ensure rows are ready */

    /* overall zero‑based offset in the concatenated sequence */
    idx_t offset = n - 2;

    /* if the term lies inside the finite part (bases 2..9) */
    if (offset < (idx_t)total_finite) {
        size_t acc = 0;
        for (int base = 2; base <= MAX_BASE; ++base) {
            if (offset < (idx_t)(acc + row_len[base])) {
                idx_t pos = offset - (idx_t)acc;
                return (seq_t)row_data[base][pos];
            }
            acc += row_len[base];
        }
    }

    /* otherwise we are in the infinite tail (base 10 and higher) */
    return (seq_t)(offset - (idx_t)total_finite);
}
