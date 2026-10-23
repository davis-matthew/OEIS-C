/*
 * A155786  Primes whose first and last digits differ by 1.
 * https://oeis.org/A155786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   absdiff_int, get_digits, next_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incremental search for primes whose first and last digits
   differ by 1, as defined in OEIS A155786.  The function is memoised
   with a static dynamic array and can be called in any order. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/get_digits.h"
#include "../utils/absdiff_int.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *good = NULL;      /* array of good primes (seq_t type) */
static size_t good_cap = 0;     /* current capacity of the array      */
static size_t good_cnt = 0;     /* number of primes stored so far    */
static idx_t next_candidate = 2ULL;   /* next integer to test */

/* Ensure the array can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (good_cap >= needed) return;
    size_t new_cap = good_cap ? good_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp;
    if (good_cap == 0) {
        tmp = seq_alloc(new_cap);
    } else {
        tmp = seq_realloc(good, good_cap, new_cap);
    }
    if (!tmp) abort();
    good = tmp;
    good_cap = new_cap;
}

/* Test whether a prime p has first and last digits differing by 1. */
static bool prime_has_diff1(uint64_t p)
{
    uint8_t buf[128];
    size_t len;
    get_digits((seq_t)p, buf, &len);      /* buf[0] = last digit, buf[len-1] = first digit */
    if (len == 0) return false;
    return absdiff_int(buf[len - 1], buf[0]) == 1;
}

/* Return the n-th term (1‑based) of OEIS A155786. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 – return 0 for safety */

    while (good_cnt < n) {
        /* Find the next prime ≥ next_candidate. */
        uint64_t p = next_prime(next_candidate);
        /* If next_candidate was not prime, next_prime gives the next one. */
        if (prime_has_diff1(p)) {
            ensure_capacity(good_cnt + 1);
            good[good_cnt++] = (seq_t)p;
        }
        next_candidate = p + 1ULL;
    }
    return good[n - 1];
}
