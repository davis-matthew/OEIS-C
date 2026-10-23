/*
 * A046510  Numbers with multiplicative persistence value 1.
 * https://oeis.org/A046510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with multiplicative persistence value 1: all integers ≥10 whose digit product ≤9. */

#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoization */
static seq_t *seq_array = NULL;  /* Array of computed terms */
static size_t seq_len  = 0;      /* Number of terms stored */
static size_t seq_cap  = 0;      /* Allocated capacity of seq_array */
static seq_t next_candidate = (seq_t)10; /* Next number to test */

/* Ensure that at least n terms are generated and stored in seq_array */
static void ensure_terms(idx_t n)
{
    while (seq_len < (size_t)n) {
        seq_t prod = (seq_t)1;
        seq_t t = next_candidate;

        /* Compute full product of digits – we cannot stop early because
           a zero occurring later can bring the product back into the
           single‑digit range. */
        while (t > (seq_t)0) {
            seq_t digit = t % (seq_t)10;
            prod *= digit;
            t /= (seq_t)10;
        }

        /* If product ≤ 9, store the number */
        if (prod <= (seq_t)9) {
            if (seq_cap <= seq_len) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 64;
                while (new_cap <= seq_len) new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
                seq_array = tmp;
                seq_cap = new_cap;
            }
            seq_array[seq_len++] = next_candidate;
        }
        /* Advance candidate for next iteration */
        next_candidate += (seq_t)1;
    }
}

/* Return the nth term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* Undefined offset – safety */
    ensure_terms(n);
    return seq_array[(size_t)(n - 1)];
}
