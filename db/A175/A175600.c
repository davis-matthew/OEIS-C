/*
 * A175600  Primes of form 4k+1 where k is a Pythagorean prime.
 * https://oeis.org/A175600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p = 4*k + 1 and k itself is a prime of the
 *            form 4*m + 1 (A175600). The routine searches for such primes on
 *            demand and memoizes them
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *seq_arr = NULL;   /* Memoized sequence values   */
static size_t seq_len = 0;      /* Number of terms stored      */
static size_t seq_cap = 0;      /* Allocated capacity          */
static seq_t next_candidate = 5;/* Next 4*k + 1 to test        */

/* Ensure the storage can hold at least 'needed' terms. */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Append a new term to the memoized sequence. */
static void add_term(seq_t val)
{
    if (seq_len + 1 > seq_cap) grow_seq(seq_len + 1);
    seq_arr[seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* Undefined for non‑positive n */
    while (seq_len < n)
    {
        seq_t p = next_candidate;  /* Candidate prime 4*k + 1   */
        next_candidate += 4;       /* Advance to next odd 4*k+1 */

        if (!is_prime_big(p))      /* p must be prime            */
            continue;

        seq_t k = (p - 1) / 4;     /* k = (p-1)/4                */
        if ((k & 3) != 1)          /* k must be ≡ 1 (mod 4)      */
            continue;

        if (!is_prime_big(k))      /* k itself must be prime     */
            continue;

        add_term(p);               /* Found a valid term         */
    }
    return seq_arr[n - 1];
}
