/*
 * A107818  Slowest increasing sequence where (product of 2 consecutive
 *          integers)-1 is prime.
 * https://oeis.org/A107818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107818: Slowest increasing sequence where
 * (product of 2 consecutive integers)-1 is prime.
 * Implementation: successively find the smallest integer m > prev
 * such that (prev*m)-1 is prime. Uses is_prime (64‑bit) and
 * isprime_big (big integer) for primality testing.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
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

static seq_t *seq = NULL;
static idx_t cap = 0;
static idx_t size = 0;

/* Ensure that seq has space for indices up to needed. */
static void ensure_cap(idx_t needed)
{
    if (cap == 0) {
        cap = 16;
        seq = seq_malloc(cap);
    }
    if (needed >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= needed) new_cap <<= 1;
        seq = seq_realloc(seq, new_cap);
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* initialise first term */
    if (size == 0) {
        ensure_cap(16);
        seq[0] = 2;
        size = 1;
    }

    if (n < size) return seq[n];

    seq_t prev = seq[size - 1];

    /* maximum representable value of seq_t in an unsigned integer */
    const unsigned __int128 MAX_SEQ_VAL = (unsigned __int128)((seq_t)-1);

    for (idx_t idx = size; idx <= n; ++idx) {
        seq_t cand = prev + 1;
        for (;;) {
            unsigned __int128 prod = (unsigned __int128)prev * (unsigned __int128)cand;
            unsigned __int128 pre_minus = prod - 1;
            bool prime = false;

            /* If the value fits in 64 bits, use the 64‑bit primality test */
            if (pre_minus <= (unsigned __int128)UINT64_MAX) {
                prime = is_prime_u64((uint64_t)pre_minus);
            }
            /* Else if it fits in seq_t, try the arbitrary‑precision test */
            else if (pre_minus <= MAX_SEQ_VAL) {
                seq_t test_val = (seq_t)pre_minus;
                prime = is_prime_seq(test_val);
            }
            /* Otherwise we cannot test; treat as non‑prime and keep searching */
            if (prime) break;
            ++cand;
        }
        seq[idx] = cand;
        prev = cand;
    }

    return seq[n];
}
