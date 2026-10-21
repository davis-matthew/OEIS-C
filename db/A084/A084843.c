/*
 * A084843  Numbers n such that no palindrome m > 0 exists with n + m also a
 *          palindrome.
 * https://oeis.org/A084843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A084843: numbers n such that no palindrome m>0 exists
   with n + m also a palindrome.  The algorithm precomputes all
   palindromes up to a fixed bound, marks every difference of two distinct
   palindromes, and then returns the n‑th positive integer that is NOT
   a marked difference. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"

/* ------------------------------------------------------------ */
/* Configuration constants - array bounds are static for speed.  */
#define MAX_B      500000ULL   /* upper bound for N considered           */
#define PAL_MAX    2000U       /* maximum number of palindromes up to MAX_B */
#define VALID_MAX  MAX_B       /* same as MAX_B, holds every non‑difference */

/* ------------------------------------------------------------ */
/* Static storage, all zero‑initialised on load.                */
static bool            diff_exists[VALID_MAX + 1] = { false };
static seq_t           palins[PAL_MAX];
static size_t          pal_cnt            = 0;
static seq_t           valid_numbers[VALID_MAX];
static size_t          valid_cnt          = 0;
static bool            data_prepared      = false;

/* ------------------------------------------------------------ */
/* Helper: generate all palindromes up to MAX_B.                 */
static void generate_palindromes(void)
{
    for (idx_t i = 1; i <= MAX_B; ++i) {
        if (is_palindrome((seq_t)i, 10)) {
            if (pal_cnt >= PAL_MAX) break;   /* safety guard */
            palins[pal_cnt++] = (seq_t)i;
        }
    }
}

/* ------------------------------------------------------------ */
/* Helper: compute all differences of two distinct palindromes
   and mark them in diff_exists.                                */
static void mark_differences(void)
{
    for (size_t i = 0; i < pal_cnt; ++i) {
        for (size_t j = i + 1; j < pal_cnt; ++j) {
            seq_t diff = palins[j] - palins[i];
            if (diff <= (seq_t)MAX_B) {
                diff_exists[(size_t)diff] = true;
            }
        }
    }
}

/* ------------------------------------------------------------ */
/* Helper: build the list of valid numbers (non‑differences).     */
static void build_valid_list(void)
{
    for (idx_t k = 1; k <= MAX_B; ++k) {
        if (!diff_exists[(size_t)k]) {
            if (valid_cnt >= VALID_MAX) break;  /* safety guard */
            valid_numbers[valid_cnt++] = (seq_t)k;
        }
    }
}

/* ------------------------------------------------------------ */
/* One‑time initialisation routine, idempotent.                   */
static void prepare_sequence_data(void)
{
    if (data_prepared) return;

    generate_palindromes();
    mark_differences();
    build_valid_list();

    data_prepared = true;
}

/* ------------------------------------------------------------ */
/* Public API                                                     */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined offset */

    prepare_sequence_data();

    if (n > valid_cnt) return (seq_t)0;   /* requested term beyond precomputed range */

    return valid_numbers[(size_t)n - 1U];
}
