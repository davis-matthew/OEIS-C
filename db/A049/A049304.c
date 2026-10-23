/*
 * A049304  Numbers k such that k is a substring of 6^k.
 * https://oeis.org/A049304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A049304: numbers k that appear as a decimal substring of 6^k. */
/* The function builds 6^k successively and checks whether the decimal
   representation of k occurs inside that power.  All computed values
   are cached, so subsequent calls are answered in O(1) time. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Dynamic arrays for caching values and for the digits of 6^k.           */
/* --------------------------------------------------------------------- */

static seq_t *cache = NULL;          /* cache of found ks                     */
static idx_t cache_len = 0;          /* number of elements currently stored   */

static uint8_t *pow_digits = NULL;   /* least‑significant digit first         */
static size_t pow_len = 0;           /* current number of used digits         */
static size_t pow_cap = 0;           /* allocated capacity for pow_digits     */

static char *pow_str = NULL;         /* string representation of 6^k         */
static size_t powstr_cap = 0;        /* allocated capacity for pow_str       */

/* --------------------------------------------------------------------- */
/* Grow the cache to hold at least `needed` elements.                      */
/* --------------------------------------------------------------------- */
static void grow_cache(size_t needed)
{
    if (cache_len >= needed) return;
    size_t new_cap = cache_len ? cache_len * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* out of memory */
    cache = tmp;
}

/* --------------------------------------------------------------------- */
/* Grow the digits array for 6^k.                                          */
/* --------------------------------------------------------------------- */
static void grow_pow_digits(size_t needed)
{
    if (pow_cap >= needed) return;
    size_t new_cap = pow_cap ? pow_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    uint8_t *tmp = (uint8_t *)realloc(pow_digits, new_cap);
    if (!tmp) abort();
    pow_digits = tmp;
    pow_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Grow the string buffer for 6^k.                                         */
/* --------------------------------------------------------------------- */
static void grow_pow_str(size_t needed)
{
    if (powstr_cap >= needed) return;
    size_t new_cap = powstr_cap ? powstr_cap * 2 : 1024;
    if (new_cap < needed) new_cap = needed;
    char *tmp = (char *)realloc(pow_str, new_cap);
    if (!tmp) abort();
    pow_str = tmp;
    powstr_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main sequence routine.                                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; behaviour below it is irrelevant */

    /* Initialise state on the very first call. */
    static bool initialized = false;
    static idx_t exponent = 0;            /* current k (k == exponent)           */

    if (!initialized) {
        /* 6^0 = 1  */
        grow_pow_digits(1);
        pow_digits[0] = 1;
        pow_len = 1;
        initialized = true;
    }

    /* Generate terms until we have at least n values cached. */
    while (cache_len < n) {
        /* Increment exponent and multiply current power by 6. */
        exponent++;

        uint64_t carry = 0;
        for (size_t i = 0; i < pow_len; ++i) {
            uint64_t prod = (uint64_t)pow_digits[i] * 6 + carry;
            pow_digits[i] = (uint8_t)(prod % 10);
            carry        = prod / 10;
        }
        while (carry > 0) {
            if (pow_len == pow_cap) grow_pow_digits(pow_len + 1);
            pow_digits[pow_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* Convert the digits of 6^k into a decimal string. */
        grow_pow_str(pow_len + 1);
        for (size_t i = 0; i < pow_len; ++i) {
            pow_str[pow_len - 1 - i] = (char)(pow_digits[i] + '0');
        }
        pow_str[pow_len] = '\0';

        /* Convert the current exponent into a decimal string. */
        char exp_str[32];
        /* idx_t is unsigned, but snprintf expects signed for %llu; cast safely. */
        snprintf(exp_str, sizeof exp_str, "%llu", (unsigned long long)exponent);

        /* Check whether exponent appears as a substring of 6^k. */
        if (strstr(pow_str, exp_str) != NULL) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)exponent;
        }
    }

    return cache[n - 1];
}
