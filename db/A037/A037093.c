/*
 * A037093  "Sloping binary representation" of Fibonacci numbers, slope = +1.
 * https://oeis.org/A037093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥0} ((F_{n+i} >> i) & 1) * 2^i, stopping * when
 *            F_{n+i} < 2^i. Here F_k is the k‑th Fibonacci number * (F_0 = 0,
 *            F_1 = 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Cached Fibonacci numbers.  fib_cache[0] = 0, fib_cache[1] = 1.  */
static seq_t *fib_cache = NULL;
static idx_t fib_count = 0;      /* number of Fibonacci entries currently cached */

/* Ensure that fib_cache holds Fibonacci numbers up to index idx inclusive. */
static void ensure_fib(idx_t idx)
{
    if (fib_cache == NULL) {
        /* Allocate space for indices 0 and 1. */
        fib_cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_count = 2;
    }

    /* Grow the cache until it contains index idx. */
    while (fib_count <= idx) {
        /* Compute the next Fibonacci number. */
        seq_t next = fib_cache[fib_count - 1] + fib_cache[fib_count - 2];

        /* Grow the allocated array if needed. */
        size_t new_cap = fib_count * 2;
        if (new_cap <= fib_count) {   /* overflow protection */
            new_cap = fib_count + 1;
        }
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();     /* allocation failure  */
        fib_cache = tmp;
        fib_cache[fib_count++] = next;
    }
}

/* Compute a(n) for the OEIS sequence A037093. */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    idx_t i      = (idx_t)0;
    seq_t mask   = (seq_t)1;
    const idx_t maxbits = (idx_t)(sizeof(seq_t) * 8);

    while (i < maxbits) {
        idx_t idx = n + i;
        ensure_fib(idx);
        seq_t f = fib_cache[idx];

        /* Stop if 2^i > F_{n+i}; i.e. if f < mask. */
        if (f < mask) {
            break;
        }

        if (f & mask) {
            result += mask;
        }

        /* Prepare mask for next i.  Stop before shifting past sizeof(seq_t)*8. */
        if (i + 1 >= maxbits) {
            break;
        }
        mask <<= 1;
        i++;
    }
    return result;
}
