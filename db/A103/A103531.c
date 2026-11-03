/*
 * A103531  Numbers k such that k235711 is prime.
 * https://oeis.org/A103531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 937 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A103531: numbers k such that k235711 is prime.
   Compute by incrementally searching for primes in the arithmetic progression
   k*10^6 + 235711. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;     /* stored terms of the sequence */
    static idx_t cap = 0;         /* current capacity of arr */
    static idx_t len = 0;         /* number of terms stored */
    static uint64_t cur_k = 1;    /* next k to test */

    /* initialise storage */
    if (arr == NULL) {
        cap = 1024;
        arr = seq_malloc(cap);
    }

    /* generate terms until we have at least n terms */
    while (len < n) {
        uint64_t cand = cur_k * 1000000ULL + 235711ULL; /* k*10^6 + 235711 */

        if (is_prime_u64(cand)) {
            if (len >= cap) {                /* grow storage if needed */
                cap <<= 1;
                arr = seq_realloc(arr, cap);
            }
            arr[len] = (seq_t)cur_k;         /* store the k value */
            ++len;
        }
        ++cur_k;
    }

    return arr[n - 1];      /* idx_t is 1‑based for the sequence */
}
