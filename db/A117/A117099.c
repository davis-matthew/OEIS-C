/*
 * A117099  Numbers n such that nextprime(2*n)>2*nextprime(n) and n is
 *          composite (if p is prime then nextprime(p)=p).
 * https://oeis.org/A117099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117099: composite n such that nextprime(2*n) > 2*nextprime(n) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

static seq_t *seq = NULL;          /* dynamic array of sequence values */
static size_t seq_len = 0;         /* number of values stored */
static size_t seq_cap = 0;         /* current array capacity */
static idx_t next_candidate = 4;   /* smallest integer to test for this sequence */

/* Ensure the array can hold at least `needed` entries. */
static void ensure_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* memory exhausted */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Generate sequence terms until at least `n` terms are available. */
static void extend_to(idx_t n)
{
    if (n <= (idx_t)seq_len)
        return;

    while (seq_len < (size_t)n) {
        idx_t m = next_candidate++;          /* next integer to test */

        if (m <= (idx_t)3)            /* 1,2,3 are not composite */
            continue;

        if (is_prime(m))             /* skip primes */
            continue;

        uint64_t p = next_prime((uint64_t)m);          /* next prime ≥ m */
        uint64_t q = next_prime((uint64_t)(m * 2ULL));  /* next prime ≥ 2*m */

        if (q > 2ULL * p) {          /* sequence condition satisfied */
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)m;
        }
    }
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* undefined for n=0, return 0 as a safe value */
        return (seq_t)0;

    extend_to(n);
    return seq[n - 1];
}
