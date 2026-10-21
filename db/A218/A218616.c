/*
 * A218616  The infinite trunk of beanstalk (A179016) with reversed
 *          subsections.
 * https://oeis.org/A218616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) computed by iteratively generating each row of the
 *            reversed-beanstalk construction. Each row n (n ≥ 1) starts with
 *            (2^n)–1 and continues by repeatedly subtracting the binary
 *            popcount until the value (2^(n‑1))–1 is reached, which is
 *            already present in the sequence and is not repeated. The fir...
 * Range:     u8 n <= 68, u16 n <= 8727, u32 n <= 8727, u64 n <= 8727,
 *            u128 n <= 8727, big any n
 * Helpers:   ensure_capacity, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp) */
#include "../utils/ensure_capacity.h" /* void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) */

/* popcount for arbitrary-precision seq_t (works for 8,16,32,64 or 128 bits). */
static inline unsigned popcount_seq_t(seq_t x)
{
    unsigned cnt = 0;
    while (x)
    {
        x &= x - (seq_t)1;
        ++cnt;
    }
    return cnt;
}

/* Generate one row of the sequence and append its terms to *seq. */
static void generate_row(idx_t r, seq_t **seq, size_t *cap, size_t *len)
{
    seq_t start = pow2(r) - (seq_t)1;               /* (2^r) – 1 */
    seq_t prev_start = pow2(r - 1) - (seq_t)1;     /* (2^(r-1)) – 1 */
    seq_t current = start;

    while (1)
    {
        /* Ensure we have enough space. */
        if (*len == *cap)
            ensure_capacity(*len + 1, cap, (void **)&(*seq), sizeof(seq_t));

        (*seq)[*len] = current;
        ++(*len);

        unsigned pc = popcount_seq_t(current);
        seq_t next = current - (seq_t)pc;

        if (next == prev_start)
            break;          /* reached value already listed; stop without adding it */
        current = next;
    }
}

/* Sequence entry point. */
seq_t a(idx_t n)
{
    /* Static storage for memoised values. */
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;
    static idx_t  next_row = 1;   /* next row to generate (starts from 1) */

    /* Initialise with the first term a(0) = 0 if not yet done. */
    if (seq_len == 0)
    {
        ensure_capacity(1, &seq_cap, (void **)&seq, sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    /* Generate rows until we have covered the requested index. */
    while (seq_len <= n)
        generate_row(next_row++, &seq, &seq_cap, &seq_len);

    return seq[n];
}
