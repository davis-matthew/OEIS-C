/*
 * A052044  Numbers k such that k^3 lacks the digit zero in its decimal
 *          expansion.
 * https://oeis.org/A052044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k (k ≥ 1) such that k³ contains no
 *            decimal digit 0. The function generates terms on demand and
 *            stores only the integer values of k (as uint64_t). When an a(n)
 *            is requested it converts the stored k to seq_t
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required types are provided by the build system. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_zero_uint128.h"

/* Array of computed k-values.  We store them as uint64_t to keep the array
   construction portable across builds where seq_t may be an arbitrary‑precision
   class that requires constructors. */
static uint64_t *list_k = NULL;
static size_t      list_len = 0;
static size_t      list_cap = 0;
static uint64_t    last_k   = 0;      /* largest k examined so far */

/* Ensure the array has room for at least 'need' elements. */
static void ensure_cap(size_t need)
{
    if (need <= list_cap) return;
    size_t new_cap = list_cap ? list_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(list_k, new_cap * sizeof(uint64_t));
    if (!tmp) abort();               /* out of memory */
    list_k   = tmp;
    list_cap = new_cap;
}

/* Generate terms until we have at least 'n' terms. */
static void generate_terms(idx_t n)
{
    while (list_len < (size_t)n) {
        last_k++;
        unsigned __int128 cube = (unsigned __int128)last_k * last_k * last_k;
        if (!has_zero_uint128(cube)) {
            ensure_cap(list_len + 1);
            list_k[list_len++] = last_k;
        }
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1; undefined for n=0 */
    if (n > (idx_t)list_len) generate_terms(n);
    return (seq_t)list_k[n - 1];
}
