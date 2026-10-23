/*
 * A108028  Numbers k such that 139*k + 137 is prime.
 * https://oeis.org/A108028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4390, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108028: numbers k such that 139*k + 137 is prime.
   a(1)=0, a(2)=6, a(3)=8, … (see OEIS entry). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/is_prime.h"

/* Static storage for the already found values of the sequence. */
static seq_t *k_values = NULL;   /* array of found k's */
static size_t cap = 0;           /* current allocated capacity */
static size_t sz  = 0;           /* number of values stored */
static uint64_t curr_k = 0;      /* smallest k not yet tested */

/* Resize the array if needed. */
static void ensure_capacity()
{
    if (sz >= cap) {
        size_t newcap = (cap < 16) ? 16 : cap * 2;
        seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newptr) return;          /* allocation failed – should not happen in tests */
        if (k_values) {
            memcpy(newptr, k_values, sz * sizeof(seq_t));
            free(k_values);
        }
        k_values = newptr;
        cap = newcap;
    }
}

/* Compute the n‑th element (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, arbitrary return */

    /* Generate values until we have at least n of them. */
    while (sz < (size_t)n) {
        uint64_t val = 139ULL * curr_k + 137ULL;   /* 139*k + 137 */
        if (is_prime(val)) {
            ensure_capacity();
            k_values[sz++] = (seq_t)curr_k;
        }
        ++curr_k;
    }

    return k_values[n - 1];
}
