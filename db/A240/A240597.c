/*
 * A240597  Numbers k such that sigma(k) == k (mod 9).
 * https://oeis.org/A240597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240597: Numbers k such that sigma(k) ≡ k (mod 9). */
/* This implementation recomputes sigma modulo 9 up to the needed
   limit each time more terms are required.  The algorithm runs in
   O(N log N) time for the current maximum needed value N.  */

/* 1) Standard headers.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 2) Static state used by the function.  */
static idx_t   current_limit = 0;   /* maximum k processed so far.    */
static uint8_t *mod9 = NULL;        /* sigma(k) mod 9 for k∈[1..limit].*/
static seq_t   *seq  = NULL;        /* array containing the sequence.  */
static size_t  seq_len = 0;         /* number of elements stored.      */

/* 3) Recomputes sigma modulo 9 for all k ≤ new_limit and rebuilds seq.  */
static void extend_up_to(idx_t new_limit)
{
    if (new_limit <= current_limit) {
        return;
    }

    /* Allocate ambient array for sigma mod 9.  */
    uint8_t *tmp = (uint8_t *)realloc(mod9, (size_t)new_limit + 1);
    if (!tmp) abort();
    mod9 = tmp;
    /* Zero initialise the array.  */
    for (idx_t i = 1; i <= new_limit; ++i) mod9[i] = 0;

    /* Sieve‑like accumulation of divisor contributions.  */
    for (idx_t i = 1; i <= new_limit; ++i) {
        uint8_t val = (uint8_t)(i % 9);          /* i mod 9  (0…8).     */
        for (idx_t j = i; j <= new_limit; j += i) {
            uint8_t sum = mod9[j] + val;
            /* Since sum ≤ 8+8 =16, a single subtraction suffices. */
            if (sum >= 9) sum -= 9;
            mod9[j] = sum;
        }
    }

    /* Count how many numbers satisfy sigma(k) ≡ k (mod 9).  */
    size_t count = 0;
    for (idx_t k = 1; k <= new_limit; ++k) {
        if (mod9[k] == (uint8_t)(k % 9)) ++count;
    }

    /* Allocate space for the sequence.  */
    seq_t *new_seq = (seq_t *)realloc(seq, count * sizeof(seq_t));
    if (!new_seq) abort();
    seq = new_seq;

    /* Record the sequence values.  */
    size_t idx = 0;
    for (idx_t k = 1; k <= new_limit; ++k) {
        if (mod9[k] == (uint8_t)(k % 9)) {
            seq[idx++] = (seq_t)k;
        }
    }
    seq_len = count;
    current_limit = new_limit;
}

/* 4) Guarantees that the array contains at least n terms.  */
static void ensure_at_least(idx_t n)
{
    /* If we already have enough, nothing to do.  */
    while (seq_len < (size_t)n) {
        idx_t new_limit;
        if (current_limit == 0) {
            new_limit = 32;              /* initial size for small n.   */
        } else {
            new_limit = current_limit << 1;   /* double the range.         */
        }
        /* A small cushion to reduce the number of extensions.  */
        if (new_limit < n * 10) new_limit = n * 10;
        extend_up_to(new_limit);
    }
}

/* 5) Public function requested by the problem statement.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined case – safe fallback.     */
    ensure_at_least(n);
    return seq[n - 1];
}
