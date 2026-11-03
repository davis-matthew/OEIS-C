/*
 * A142915  Primes congruent to 46 mod 63.
 * https://oeis.org/A142915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 956 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is:  a(n) = the nth prime p with p ≡ 46 (mod 63)
   (offset 1, a(1)=109, a(2)=487, …) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Cached list of primes ≡ 46 (mod 63).  The array is sorted and
   grows on demand. */
static seq_t *primes   = NULL;
static size_t primes_len = 0;  /* number of stored primes  */
static size_t primes_cap = 0;  /* current allocation size    */

/* The value of the last number tested that is ≡ 46 (mod 63).
   It starts at 46 and is advanced by 63 each iteration. */
static idx_t last_checked = 46;

/* Ensure that at least n primes congruent to 46 (mod 63) are stored. */
static void ensure_at_least(idx_t n)
{
    while (primes_len < (size_t)n) {
        last_checked += 63;                     /* next candidate   */
        /* is_prime works for idx_t (i.e. uint64_t) and returns a bool */
        if (is_prime((idx_t)last_checked))
            append_candidate(&primes, &primes_len, &primes_cap,
                             (seq_t)last_checked);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n ≥ 1; return 0 for safety */
        return (seq_t)0;
    ensure_at_least(n);
    return primes[n - 1];
}
