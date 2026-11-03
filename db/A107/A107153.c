/*
 * A107153  Primes of the form 2x^2 + 23y^2.
 * https://oeis.org/A107153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 2x^2 + 23y^2. See OEIS A107153
 * Range:     u8 n <= 8, u16 n <= 811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 7842 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

static bool is_representable(uint64_t p) {
    for (uint64_t y = 0; ; ++y) {
        unsigned __int128 term = (unsigned __int128)23 * ((unsigned __int128)y * (unsigned __int128)y);
        if (term > p) break;
        unsigned __int128 rem = (unsigned __int128)p - term;
        if ((rem & 1u) == 0) {
            uint64_t t = (uint64_t)(rem >> 1);      /* t = rem/2 */
            uint64_t s = isqrt(t);
            if ((unsigned __int128)s * s == t) return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *arr   = NULL;
    static idx_t  size  = 0;
    static idx_t  cap   = 0;
    static idx_t  curr  = 2;   /* current candidate (uint64_t when used) */

    while (size < n) {
        uint64_t p = (uint64_t)curr;
        if (is_prime_u64(p) && is_representable(p))
            push_seq(&arr, &cap, &size, (seq_t)p);

        /* advance to next odd number after 2 */
        if (curr == 2) curr = 3;
        else           curr += 2;
    }
    return arr[n - 1];
}
