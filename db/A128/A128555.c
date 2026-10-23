/*
 * A128555  a(n) = the smallest positive multiple of d(n) that does not occur
 *          earlier in the sequence, where d(n) is the number of positive
 *          divisors of n.
 * https://oeis.org/A128555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A128555
 * a(n) = smallest multiple of d(n) that has not appeared in the
 * sequence before.  d(n) is the number of positive divisors of n.
 * The sequence is a permutation of the positive integers.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"

/*--- mutable static state -------------------------------------------------*/
static seq_t *seq          = NULL;   /* array of computed a(k) values      */
static size_t seq_cap      = 0;      /* allocated capacity of seq          */
static size_t seq_len      = 0;      /* number of terms currently computed */

static uint8_t *seen_bitmap = NULL; /* bit‑set of values that have already been used */
static size_t seen_bits     = 0;     /* number of bits currently allocated */

/*--- helpers --------------------------------------------------------------*/
static void ensure_seq_capacity(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap * 2 : 16;
    if (cap < new_cap) cap = new_cap;

    seq_t *tmp = (seq_t*)realloc(seq, cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = cap;
}

static void ensure_seen_capacity(idx_t val)
{
    /* number of bits needed to index *val*   */
    size_t needed_bits = (size_t)val + 1;
    if (needed_bits <= seen_bits) return;

    size_t old_bits = seen_bits;
    size_t new_bits = seen_bits ? seen_bits * 2 : 128;
    if (new_bits < needed_bits) new_bits = needed_bits;
    seen_bits = new_bits;

    size_t old_bytes = old_bits >> 3;                      /* floor(old_bits/8) */
    size_t new_bytes = (new_bits + 7) >> 3;                /* ceil(new_bits/8)  */
    uint8_t *tmp = (uint8_t*)realloc(seen_bitmap, new_bytes);
    if (!tmp) exit(EXIT_FAILURE);
    /* zero the newly added region */
    if (new_bytes > old_bytes)
        memset(tmp + old_bytes, 0, new_bytes - old_bytes);
    seen_bitmap = tmp;
}

static inline bool seen_get(idx_t v)
{
    return (seen_bitmap[v >> 3] >> (v & 7)) & 1;
}

static inline void seen_set(idx_t v)
{
    seen_bitmap[v >> 3] |= (uint8_t)(1U << (v & 7));
}

/*--- main function -------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n = 0, but guard */

    /* if already computed, return */
    if (seq_len >= n)
        return seq[n - 1];

    /* generate terms up to n */
    for (idx_t cur = seq_len + 1; cur <= n; ++cur)
    {
        uint64_t dn = divisor_count_u64((uint64_t)cur);   /* d(cur)   */
        seq_t mult = (seq_t)dn;                          /* start at dn*1 */

        /* find smallest unused multiple of dn */
        while (1)
        {
            ensure_seen_capacity((idx_t)mult);
            if (!seen_get((idx_t)mult)) break;
            mult += (seq_t)dn;
        }

        /* record the new term */
        ensure_seq_capacity(seq_len + 1);
        ensure_seen_capacity((idx_t)mult);
        seen_set((idx_t)mult);
        seq[seq_len++] = mult;
    }
    return seq[n - 1];
}
