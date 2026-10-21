/*
 * A210018  In base 7, numbers n which have 7 distinct digits, do not start
 *          with 0, and have property that the product (written in base 7) of
 *          any two adjacent digits is a substring of n.
 * https://oeis.org/A210018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   prod_is_substring, prod_to_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search over all permutations of digits 0–6 to
   find numbers satisfying the adjacency product‑substring condition.
   The resulting decimal numbers are cached once and returned for
   requested indices. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/prod_is_substring.h"
#include "../utils/prod_to_base7.h"

/* maximum number of 7‑digit permutations */
#define MAX_7DIG_PERM 5040

/* cache of valid terms */
static seq_t  valid_seq[MAX_7DIG_PERM];
static size_t valid_len = 0;
static bool   initialized = false;

/* Check whether the 7‑digit sequence d satisfies the adjacency
   product‑substring property. */
static bool is_valid(const uint8_t d[7])
{
    if (d[0] == 0) return false;            /* cannot start with 0 */
    for (int i = 0; i < 6; ++i) {
        if (!prod_is_substring(d, i)) return false;
    }
    return true;
}

/* Recursive generation of all permutations of digits 0–6. */
static void rec_gen(int pos, uint8_t mask, uint8_t digit[7])
{
    if (pos == 7) {
        if (is_valid(digit)) {
            /* interpret the digits as a decimal number */
            seq_t val = 0;
            for (int j = 0; j < 7; ++j)
                val = val * 10 + digit[j];
            if (valid_len < MAX_7DIG_PERM)
                valid_seq[valid_len++] = val;
        }
        return;
    }
    for (uint8_t d = 0; d < 7; ++d) {
        if (!(mask & (1 << d))) {
            digit[pos] = d;
            rec_gen(pos + 1, mask | (1 << d), digit);
        }
    }
}

/* Initialise the cache once.  The list is generated in lexicographic
   order, which is numerically sorted for fixed length base‑7 numbers. */
static void init_sequence(void)
{
    if (initialized) return;
    uint8_t digit[7];
    rec_gen(0, 0, digit);
    initialized = true;
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined index */
    init_sequence();
    if (n - 1 < valid_len)
        return valid_seq[n - 1];
    return 0;             /* index out of range */
}
