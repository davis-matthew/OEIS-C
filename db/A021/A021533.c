/*
 * A021533  Decimal expansion of 1/529.
 * https://oeis.org/A021533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the decimal expansion of 1/529.
   The digit at position n (starting from 0) is returned.
   The sequence is a(n) = the nth digit of 1/529. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *exp_digits = NULL;   /* cached digits */
static idx_t cache_size   = 0;     /* number of digits cached */
static idx_t cache_cap    = 0;     /* allocated capacity */
static uint64_t rem        = 1;    /* current remainder (0 < rem < 529) */

static void grow_cache(idx_t up_to)
{
    if (up_to < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= up_to)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(exp_digits, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* Out of memory – in a test environment this is acceptable. */
        exit(EXIT_FAILURE);
    }
    exp_digits = tmp;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        /* Cache enough digits up to n */
        grow_cache(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            rem *= 10ULL;
            exp_digits[i] = (seq_t)(rem / 529ULL);
            rem           = rem % 529ULL;
        }
        cache_size = n + 1;
    }
    return exp_digits[n];
}
