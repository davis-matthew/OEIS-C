/*
 * A100959  Non-semiprimes.
 * https://oeis.org/A100959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer that is NOT a semiprime (i.e., the
 *            number does not have exactly two prime factors, counting
 *            multiplicities)
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9865 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

/* Grow an array of seq_t elements to at least min_cap elements. */
static void ensure_capacity(size_t min_cap, seq_t **arr, size_t *cap)
{
    if (min_cap <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 1024;
    if (new_cap < min_cap) new_cap = min_cap;
    *arr = mem_resize(seq_t, *arr, new_cap);
    *cap = new_cap;
}

/* Return the n-th non‑semiprime (a(1)=1, a(2)=2, …).  The function
   builds a static array of the found values on the first call and
   extends it lazily on subsequent calls, so each value is computed
   only once. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *non_semiprimes = NULL;  /* array of found values   */
    static idx_t   count   = 0;           /* number of values stored */
    static idx_t   next    = 1;           /* next integer to test    */
    static size_t  cap     = 0;           /* current array capacity  */

    if (n == 0)                     /* offset of sequence is 1 */
        return 0;

    /* generate values until we have at least n of them */
    while (count < n) {
        uint64_t v = next;          /* candidate integer       */
        if (!is_semiprime_uint64(v)) { /* not a semiprime */
            /* ensure we have enough room in the array */
            ensure_capacity(count + 1, &non_semiprimes, &cap);
            non_semiprimes[count] = (seq_t)v;
            ++count;
        }
        ++next;
    }
    return non_semiprimes[n - 1];
}
