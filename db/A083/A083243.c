/*
 * A083243  Numbers k for which there are more divisors and coprimes than
 *          other numbers less than k: A045763(k) < A073757(k) or A045763(k) <
 *          k/2 or A073757(k) > k/2.
 * https://oeis.org/A083243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that d(k)+phi(k)-1 > k/2
 * (d(k) = divisor count, phi(k) = Euler totient) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

/* Dynamic array holding found terms. */
static seq_t *seq = NULL;
static size_t seq_len = 0;   /* number of stored terms */
static size_t seq_cap = 0;   /* current capacity */
static uint64_t next_k = 0;  /* largest k that has been tested */

/* Ensure the array has at least “need” slots. */
static void grow_seq(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Append a new qualifying term to the array. */
static void append(seq_t value)
{
    grow_seq(seq_len + 1);
    seq[seq_len++] = value;
}

/* Return true iff k satisfies the defining inequality. */
static bool qualifies(uint64_t k)
{
    seq_t phi_k = phi(k);                 /* φ(k) */
    unsigned d_k = divisor_count_u64(k); /* d(k) */
    seq_t lhs = (seq_t)d_k + phi_k - 1;   /* d + φ - 1 */
    lhs = lhs * 2;                        /* compare with k */
    return lhs > (seq_t)k;
}

/* ------------------------------------------------------------------ */
/*  a(n) – nth term of the sequence A083243 (1‑based indexing).      */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, use 0 as sentinel */

    while (seq_len < (size_t)n) {
        ++next_k;
        if (qualifies(next_k))
            append((seq_t)next_k);
    }

    return seq[n - 1];
}
