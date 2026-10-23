/*
 * A049510  Numbers k such that prime(k) == 9 (mod 10).
 * https://oeis.org/A049510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049510: a(n) = the index k of the nth prime such that
   prime(k) ≡ 9 (mod 10).  Computed by incrementally generating primes
   and storing the corresponding indices. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *indices = NULL;          /* array of a(n) values */
static size_t indices_len = 0;           /* number of values stored */
static size_t indices_cap = 0;           /* capacity of the array */
static uint64_t current_candidate = 2;   /* next integer to test for primality */
static size_t prime_count = 0;           /* current count of primes (index) */

static void ensure_indices_cap(size_t needed) {
    if (indices_cap >= needed) return;
    size_t new_cap = indices_cap == 0 ? 16 : indices_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(indices, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    indices = tmp;
    indices_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, but return 0 */
    while (indices_len < (size_t)n) {
        if (current_candidate > UINT64_MAX) abort();
        if (is_prime(current_candidate)) {
            ++prime_count;                      /* prime at position prime_count */
            if (current_candidate % 10 == 9) {   /* ends with digit 9 */
                ensure_indices_cap(indices_len + 1);
                indices[indices_len++] = (seq_t)prime_count;
            }
        }
        ++current_candidate;
    }
    return indices[n - 1];
}
