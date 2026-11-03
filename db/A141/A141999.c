/*
 * A141999  Primes congruent to 23 mod 29.
 * https://oeis.org/A141999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) returns the n-th prime congruent to 23 (mod 29)
 * Range:     u8 n <= 3, u16 n <= 232, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 953 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates primes of the form 29k+23 by simple
 * trial division using the helper function isPrimeGeneric.  It keeps a
 * static dynamic array of found primes and grows it as needed.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t *primes_arr   = NULL;      /* array of found primes   */
static size_t primes_cap   = 0;          /* current capacity       */
static size_t primes_len   = 0;          /* number of stored primes*/

static seq_t last_checked = 23;          /* next candidate 29k+23  */

/* Ensure that at least n primes in the desired progression are stored */
static void ensure_primes(idx_t n)
{
    while (primes_len < n) {
        seq_t cand = last_checked;
        if (isPrimeGeneric(cand)) {
            /* Grow the array if needed */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap == 0 ? 8 : primes_cap * 2;
                seq_t *tmp = mem_resize(seq_t, primes_arr, new_cap);
                if (!tmp) {          /* memory failure – abort safely */
                    exit(EXIT_FAILURE);
                }
                primes_arr = tmp;
                primes_cap = new_cap;
            }
            primes_arr[primes_len++] = cand;
        }
        last_checked += 29;             /* next candidate 29k+23*/
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;               /* undefined for n<1, return 0 */
    ensure_primes(n);
    return primes_arr[n - 1];
}
