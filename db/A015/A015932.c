/*
 * A015932  Positive integers n such that 2^n == 2^10 (mod n).
 * https://oeis.org/A015932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015932: positive integers n such that 2^n ≡ 1024 (mod n).
   The function computes terms by searching successive integers and
   storing the ones that satisfy the congruence. */

#include <stdlib.h>
#include "../utils/mod_pow.h"   /* provides seq_t mod_pow(seq_t, uint64_t, seq_t) */

static seq_t *seq = NULL;      /* cached terms */
static size_t seq_len = 0;     /* number of cached terms */
static size_t seq_cap = 0;     /* capacity of cache */
static idx_t next_candidate = 1; /* next integer to test */

/* Ensure the cache can hold at least needed elements. */
static void ensure_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* maximum value that can be stored in seq_t */
    const seq_t max_seq = (seq_t)(~(seq_t)0);

    while (seq_len < (size_t)n)
    {
        if (next_candidate > (idx_t)max_seq) {
            /* No more numbers can be represented in seq_t. */
            return 0;
        }
        seq_t cand_s = (seq_t)next_candidate;

        /* Compute 2^cand mod cand. */
        seq_t left = mod_pow((seq_t)2, (uint64_t)next_candidate, cand_s);

        /* Compute 1024 mod cand. */
        seq_t right = (seq_t)(1024 % (uint64_t)cand_s);

        if (left == right) {
            ensure_cap(seq_len + 1);
            seq[seq_len++] = cand_s;
        }
        ++next_candidate;
    }
    return seq[n-1];
}
