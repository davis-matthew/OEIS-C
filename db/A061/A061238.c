/*
 * A061238  Prime numbers == 2 (mod 9).
 * https://oeis.org/A061238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A061238 by checking numbers of the form 9*k + 2
   (i.e. prime numbers that are 2 modulo 9).  a(1)=2, a(2)=11, …   */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static seq_t *list = NULL;      /* array of found primes */
static size_t  len   = 0;       /* number of primes stored */
static size_t  cap   = 0;       /* capacity of the array */
static idx_t   next  = 2;       /* next integer to test for primality */

/* Expand the storage array to hold at least 'need' elements. */
static void ensure_capacity(size_t need) {
    if (need <= cap) return;
    size_t new_cap = cap ? cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp;
#if defined(__cplusplus)
    tmp = new seq_t[new_cap];
    if (list) {
        for (size_t i = 0; i < len; ++i) tmp[i] = list[i];
        delete[] list;
    }
#else
    tmp = realloc(list, new_cap * sizeof(seq_t));
#endif
    if (!tmp) abort();
    list = tmp;
    cap  = new_cap;
}

/* Generate primes until at least 'target' primes of residue 2 mod 9 have been found. */
static void generate_up_to(idx_t target) {
    while (len < (size_t)target) {
        if (next % 9 == 2) {
            if (is_prime(next)) {
                ensure_capacity(len + 1);
                list[len++] = (seq_t)next;
            }
        }
        ++next;
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n < 1 */
    generate_up_to(n);
    return list[n - 1];
}
