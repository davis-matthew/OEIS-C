/*
 * A106075  Primes p such that 3*p + 26 and 26*p + 3 are primes.
 * https://oeis.org/A106075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 3*p+26 and 26*p+3 are primes. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static seq_t *good_list = NULL;
static idx_t good_cnt   = 0;
static idx_t good_cap   = 0;
static idx_t last_checked = 1;      /* next integer to test */

/* Append a new good prime to the memoised list, enlarging the array if needed. */
static void add_good(seq_t val)
{
    if (good_cnt >= good_cap) {
        size_t new_cap = good_cap ? good_cap * 2 : 8;
        seq_t *new_arr = (seq_t *)realloc(good_list, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);                     /* simple error handling */
        good_list = new_arr;
        good_cap = (idx_t)new_cap;
    }
    good_list[good_cnt++] = val;
}

/* Ensure that at least n terms have been found. */
static void ensure(idx_t n)
{
    if (n == 0) return;
    while (good_cnt < n) {
        /* Find the next prime candidate. */
        if (last_checked < 2)
            last_checked = 2;
        else if ((last_checked & 1) == 0)   /* even */
            ++last_checked;                 /* make it odd */

        /* Skip non‑primes. */
        while (!is_prime((uint64_t)last_checked)) {
            if (last_checked == 2)
                last_checked = 3;
            else
                last_checked += 2;
        }

        uint64_t p = last_checked;

        /* Compute the two linear expressions. */
        unsigned __int128 v1 = (unsigned __int128)3 * p + 26;
        unsigned __int128 v2 = (unsigned __int128)26 * p + 3;

        /* Test the conditions if the values fit in 64 bits. */
        if (v1 <= UINT64_MAX && v2 <= UINT64_MAX) {
            if (is_prime((uint64_t)v1) && is_prime((uint64_t)v2))
                add_good((seq_t)p);
        }

        /* Advance to the next candidate for the next iteration. */
        if (p == 2)
            last_checked = 3;
        else
            last_checked += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */
    ensure(n);
    return good_list[n - 1];
}
