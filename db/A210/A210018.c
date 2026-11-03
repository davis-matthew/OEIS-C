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
 * Validated: full, pass, 62 terms through n = 62 (2026-11-02)
 * Helpers:   none
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

/* Convert product of two adjacent digits to an array of base‑7 digits.
   sub  : buffer that will contain the result digits (most significant first)
   sublen: pointer to store the number of digits written */
static void prod_to_base7(uint8_t prod, uint8_t sub[3], int *sublen)
{
    if (prod == 0) {
        sub[0] = 0;
        *sublen = 1;
        return;
    }
    /* compute in reverse order */
    int len = 0;
    while (prod > 0) {
        sub[len++] = (uint8_t)(prod % 7);
        prod /= 7;
    }
    /* reverse to get most significant first */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t tmp = sub[i];
        sub[i] = sub[len - 1 - i];
        sub[len - 1 - i] = tmp;
    }
    *sublen = len;
}

/* Check whether the product of two adjacent digits is a substring of
   the 7‑digit sequence d. */
static bool prod_is_substring(const uint8_t d[7], int i)
{
    uint8_t prod = d[i] * d[i + 1];
    uint8_t sub[3];
    int sublen = 0;

    prod_to_base7(prod, sub, &sublen);

    /* search for sub in d */
    for (int start = 0; start <= 7 - sublen; ++start) {
        bool ok = true;
        for (int k = 0; k < sublen; ++k) {
            if (d[start + k] != sub[k]) {
                ok = false;
                break;
            }
        }
        if (ok) return true;
    }
    return false;
}

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
