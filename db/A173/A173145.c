/*
 * A173145  Numbers n such that neither prime(n-1) nor prime(n+1) is of form
 *          6k-1.
 * https://oeis.org/A173145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A173145:
 * a(n) = the n‑th integer k (k ≥ 3) for which neither
 * prime(k‑1) nor prime(k+1) is congruent to 5 modulo 6.
 * The sequence starts with 3,5,7,13,20,26,... 
 *
 * The routine uses a 1‑based index n (n ≥ 1) and returns the
 * corresponding term a(n) as a value of type seq_t.
 * It memoises previously computed terms so that any call order
 * works efficiently for indices up to about 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* ------------------------------------------------------------------ */
/* Memoisation buffer for the sequence values */
static seq_t *seq_cache   = NULL;  /* array of a(i) */
static size_t seq_len     = 0;     /* number of terms stored */
static size_t seq_cap     = 0;     /* allocated capacity */

/* The next integer to test for membership in the sequence */
static idx_t next_candidate = 3;   /* first admissible integer (a(1)=3) */

/* ------------------------------------------------------------------ */
/* Ensure the sequence buffer has room for at least 'need' elements. */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public function: return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n = 0 */

    /* Generate terms until we have the required index n. */
    while (seq_len < (size_t)n)
    {
        /* Access the (k‑1)-th and (k+1)-th primes with k = next_candidate. */
        uint32_t p_prev = get_prime(next_candidate - 1);  /* prime(k-1) */
        uint32_t p_next = get_prime(next_candidate + 1);  /* prime(k+1) */

        /* Condition: neither prime is 6k-1 ⇔ %6 != 5. */
        if ((p_prev % 6u) != 5u && (p_next % 6u) != 5u)
        {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;      /* move to the next integer */
    }

    return seq_cache[n - 1];   /* a(1) is stored at seq_cache[0] */
}
