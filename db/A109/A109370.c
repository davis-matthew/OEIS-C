/*
 * A109370  Numbers n such that the string 22n is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A109370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 151, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 72 terms through n = 1996 (2026-11-03)
 * Helpers:   is_prime_seq, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth ≠ integer n such that the decimal concatenation
   of 22 and n yields a prime.  In formulas:
   a(n) is the smallest integer such that 22·10^d + n is prime,
   where d is the number of decimal digits of n, and
   a(n) is the n‑th such integer (OEIS offset 1). */

/* Remark: This implementation uses a simple linear search,
   storing the found values in a dynamically resizable array.
   The routine runs in well under two seconds for n up to
   about 10 000. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

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

/* static state for memoisation */
static seq_t *cache = NULL;     /* array of found values */
static idx_t  cap   = 0;        /* current capacity of cache   */
static idx_t  size  = 0;        /* number of values stored     */
static idx_t  checked = 0;      /* largest n that has been
                                   examined for primality */

/* Append the next qualifying n to the cache.  The function
   expands the cache as needed. */
static void extend_to(idx_t target)
{
    while (size < target) {
        /* Next candidate integer to test */
        ++checked;

        /* Determine how many decimal digits n has */
        idx_t d = num_digits_idx(checked);

        /* Construct the number 22·10^d + n */
        seq_t pow10v  = pow10((idx_t)d);
        seq_t candidate = (seq_t)22 * pow10v + (seq_t)checked;

        /* If candidate is prime, remember n */
        if (is_prime_seq(candidate)) {
            push_seq(&cache, &cap, &size, (seq_t)checked);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset 1 */
    extend_to(n);                  /* ensure we have at least n values */
    return cache[n-1];             /* 1‑based index → 0‑based array */
}
