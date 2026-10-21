/*
 * A062522  5^a(n) is smallest nonnegative power of 5 containing the string
 *          'n'.
 * https://oeis.org/A062522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   mul5_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A062522
 *
 * a(n) = the smallest nonnegative integer k such that the decimal
 * representation of n occurs as a contiguous substring in the
 * decimal representation of 5^k.
 *
 * The function keeps a cache of already computed powers of 5 as
 * strings and of previously computed a(n) values.  This permits
 * fast evaluation of any a(n) in any order of calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "../utils/mul5_string.h"

/* -------------- helper functions to handle powers of 5 ------------ */

/* dynamic array to hold powers of 5 as strings, indexed by k */
static char **powers = NULL;
static size_t powers_cap = 0;
static size_t powers_size = 0;

/* ensure powers up to index k are computed */
static void ensure_powers_up_to(idx_t k)
{
    if (k < powers_size) return;

    /* grow array capacity if needed */
    if (powers_size == powers_cap) {
        size_t new_cap = powers_cap ? powers_cap * 2 : 8;
        char **new_arr = (char **)realloc(powers, new_cap * sizeof(char *));
        if (!new_arr) return; /* silently ignore allocation failure */
        powers = new_arr;
        powers_cap = new_cap;
    }

    /* compute missing powers */
    while (powers_size <= k) {
        if (powers_size == 0) {
            powers[powers_size++] = strdup("1");   /* 5^0 = 1 */
        } else {
            char *next = mul5_string(powers[powers_size - 1]);
            powers[powers_size++] = next;
        }
    }
}

/* -------------- cache for a(n) values -------------------------------- */

static seq_t *a_cache = NULL;      /* cached a(n) values */
static bool *a_computed = NULL;    /* flags indicating if a_cache[i] is valid */
static size_t a_cap = 0;
static size_t a_size = 0;

/* ensure cache can hold index up to n */
static void ensure_a_cache(idx_t n)
{
    if (n < a_size) return;
    size_t new_cap = a_cap ? a_cap : 256;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    bool *new_flag  = (bool  *)realloc(a_computed, new_cap * sizeof(bool));
    if (!new_cache || !new_flag) return; /* ignore failure */

    for (size_t i = a_size; i < new_cap; ++i) new_flag[i] = false;
    a_cache   = new_cache;
    a_computed = new_flag;
    a_cap     = new_cap;
    a_size    = new_cap;
}

/* -------------- main sequence function ----------------------------- */

seq_t a(idx_t n)
{
    /* return cached value if available */
    ensure_a_cache(n);
    if (a_computed[n]) return a_cache[n];

    /* convert n to decimal string */
    char n_str[32];
    snprintf(n_str, sizeof n_str, "%" PRIu64, (uint64_t)n);

    /* search for first k with n_str inside 5^k */
    for (idx_t k = 0; ; ++k) {
        ensure_powers_up_to(k);
        if (strstr(powers[k], n_str)) {
            seq_t result = (seq_t)k;
            a_cache[n] = result;
            a_computed[n] = true;
            return result;
        }
    }
}
