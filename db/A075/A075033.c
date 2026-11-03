/*
 * A075033  Numbers k such that tau(k) <= tau(k+1) <= tau(k+2) <= tau(k+3)
 *          where tau(k) = number of divisors of k.
 * https://oeis.org/A075033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k for which τ(k) ≤ τ(k+1) ≤ τ(k+2) ≤
 *            τ(k+3) (search)
 * Range:     u8 n <= 26, u16 n <= 3920, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8862 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

static void seq_free(seq_t *p) { mem_free(p); }

/* In pure C build, use malloc/free. */
static seq_t *seq_malloc(size_t n) { return mem_new(seq_t, n); }

#if defined(__cplusplus)
    
    
#else
    
    
#endif

static seq_t       *terms   = NULL;   /* cached sequence values          */
static size_t       terms_size = 0;   /* number of elements stored       */
static size_t       terms_cap  = 0;   /* allocated capacity (elem count) */
static uint64_t     next_k      = 1;   /* next integer to test */

/* Ensure the cache can hold at least min_cap elements. */
static void ensure_cap(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
    if (new_cap < min_cap) new_cap = min_cap;

    seq_t *new_ptr = seq_malloc(new_cap);
    /* Copy existing elements to the new array. */
    for (size_t i = 0; i < terms_size; ++i)
        new_ptr[i] = terms[i];

    if (terms)
        seq_free(terms);
    terms      = new_ptr;
    terms_cap  = new_cap;
}

/* Return the n‑th element of the OEIS A075033 sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safe fallback – offset is 1 */

    while (terms_size < (size_t)n)
    {
        uint64_t k   = next_k;
        uint64_t d0  = num_divisors(k);
        uint64_t d1  = num_divisors(k + 1);
        uint64_t d2  = num_divisors(k + 2);
        uint64_t d3  = num_divisors(k + 3);

        if (d0 <= d1 && d1 <= d2 && d2 <= d3)
        {
            ensure_cap(terms_size + 1);
            terms[terms_size++] = (seq_t)k;
        }
        ++next_k;   /* advance to the next integer to test */
    }

    return terms[n - 1];
}
