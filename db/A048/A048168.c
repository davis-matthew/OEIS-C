/*
 * A048168  n is divisible by the square of the number of unitary divisors of
 *          n (A034444).
 * https://oeis.org/A048168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1873, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A048168 – Numbers m such that m is divisible by the square
   of the number of unitary divisors of m.
   Formula: a(n) = the n‑th integer m with  m % (4^ω(m)) == 0,
   where ω(m) is the number of distinct prime factors of m. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Static storage for the generated sequence and factorisation buffers. */
static seq_t *seq_list = NULL;         /* array of found values               */
static size_t  seq_cap  = 0;           /* allocated capacity                   */
static size_t  seq_len  = 0;           /* number of stored values              */
static uint64_t next_candidate = 1;    /* next integer to evaluate            */

static uint64_t fact_pr[64];           /* temporary prime array for factor_u64*/
static uint64_t fact_ex[64];           /* temporary exponent array            */

/* Append a value to the list, expanding the array if necessary. */
static void seq_append(seq_t val)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 256 : seq_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(seq_list, new_cap * sizeof(seq_t));
        if (!new_ptr) return;                     /* silently abort on OOM   */
        seq_list = new_ptr;
        seq_cap = new_cap;
    }
    seq_list[seq_len++] = val;
}

/* Return the number of distinct prime factors ω(n) using the supplied
   factorisation routine. */
static idx_t omega_of(uint64_t n)
{
    size_t cnt;
    factor_u64(n, fact_pr, fact_ex, &cnt);
    return cnt;
}

/* Ensure that the list contains at least `need` elements. */
static void ensure(idx_t need)
{
    while (seq_len < need) {
        uint64_t m     = next_candidate++;
        idx_t    omega = omega_of(m);
        /* compute 4^omega as a seq_t value. */
        seq_t pow = 1;
        for (idx_t i = 0; i < omega; ++i)
            pow *= (seq_t)4;
        if ((seq_t)m % pow == 0)
            seq_append((seq_t)m);
    }
}

/* Public sequence accessor. */
seq_t a(idx_t n)
{
    /* The OEIS uses 1‑based indexing; behaviour for n < 1 is undefined. */
    ensure(n);
    return seq_list[n - 1];
}
