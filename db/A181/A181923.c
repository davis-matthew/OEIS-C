/*
 * A181923  Nonprimes (A018252) mod 2.
 * https://oeis.org/A181923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k(n) mod 2), where k(n) is the nth non‑prime number *
 *            (non‑prime numbers are the positive integers that are not prime,
 *            * with 1 counted as a non‑prime). Thus a(n) is 1 when the nth *
 *            non‑prime is odd and 0 when it is even. The sequence starts *
 *            with a(1)=1, a(2)=0, a(3)=0, a(4)=0, a(5)=1,...
 * Range:     u8 n <= 121, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static array of nth non‑prime values (starting at index 0 for n=1) */
    static seq_t *nonprimes = NULL;
    static size_t size   = 0;   /* number of elements stored */
    static size_t cap    = 0;   /* current allocation size */
    static uint64_t next_candidate = 1;   /* next integer to test */

    /* grow the array when needed */
    if (size < (size_t)n) {
        /* ensure capacity for n elements */
        if (cap < (size_t)n) {
            size_t new_cap = cap == 0 ? 16 : cap * 2;
            while (new_cap < (size_t)n)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(nonprimes, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* allocation failure is fatal */
            nonprimes = tmp;
            cap = new_cap;
        }
        /* extend the cache to n elements */
        for (size_t i = size; i < (size_t)n; ++i) {
            /* find the next non‑prime */
            while (is_prime((idx_t)next_candidate))
                ++next_candidate;
            nonprimes[i] = (seq_t)next_candidate;
            ++next_candidate;   /* next search starts just after the found value */
        }
        size = (size_t)n;
    }

    return nonprimes[(size_t)n - 1] & 1;   /* parity of the nth non‑prime */
}
