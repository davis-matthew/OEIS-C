/*
 * A107618  Primes with digit sum = 64.
 * https://oeis.org/A107618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the nth prime whose digit sum is 64
 * Range:     u8 none, u16 none, u32 n <= 98133, u64 n <= 98133,
 *            u128 n <= 98133, big any n
 * Validated: standard, pass, 42 terms through n = 9789 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_u64.h"
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

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;   /* array of primes with digit sum 64 */
    static idx_t cap = 0;          /* current capacity of the array    */
    static idx_t size = 0;         /* number of primes discovered       */
    static uint64_t next = 3;      /* next odd number to test           */

    /* Find enough primes to satisfy the requested index */
    while (size < n) {
        uint64_t cand = next;
        next += 2;                  /* iterate over odd numbers only */

        if (digit_sum_u64(cand) == 64 && is_prime_u64(cand)) {
            push_seq(&primes, &cap, &size, (seq_t)cand);
        }
    }

    return primes[n - 1];
}
