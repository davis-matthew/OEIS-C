/*
 * A050000  a(n) = floor(a(n-1)/2) if this is not among 0, a(1), ..., a(n-2);
 *          otherwise a(n) = 3*a(n-1).
 * https://oeis.org/A050000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1)=1; For n>1, a(n) = floor(a(n-1)/2) if this value (and 0)
 *            has NOT appeared in a(1),...,a(n-2); otherwise a(n) = 3*a(n-1).
 *            This implements the MD(3,2) sequence A050000
 * Range:     u8 n <= 30, u16 n <= 779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static storage for computed terms and a simple bitmap of seen values. */
    static seq_t *seq = NULL;          /* array of sequence values                */
    static size_t seq_cap = 0, seq_len = 0;   /* capacity and number of values stored   */

    static uint8_t *seen = NULL;      /* bitmap: seen[v] == 1 if value v has occurred     */
    static size_t seen_cap = 0;       /* maximum value currently indexed in seen array     */

    /* Initialisation of the first term. */
    if (seq_len == 0) {
        /* Allocate room for the first term. */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;

        /* Allocate the seen bitmap, initially covering value 0 and 1. */
        seen_cap = 1;
        seen = (uint8_t *)malloc((seen_cap + 1) * sizeof(uint8_t));
        memset(seen, 0, seen_cap + 1);
        seen[1] = 1;   /* value 1 has been seen */
    }

    /* Expand the sequence until we have at least n terms. */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        seq_t cand = prev / 2;

        bool halve_ok = false;
        if (cand != (seq_t)0) {
            if ((size_t)cand > seen_cap) {
                halve_ok = true;                     /* candidate not seen yet   */
            } else if (!seen[(size_t)cand]) {
                halve_ok = true;                     /* candidate unseen        */
            }
            /* else halve_ok remains false (candidate already seen) */
        }
        /* if cand == 0, halve_ok stays false (0 is forbidden) */

        seq_t new_val;
        if (halve_ok)
            new_val = cand;
        else
            new_val = prev * (seq_t)3;

        /* Ensure capacity for the new term. */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }
        seq[seq_len] = new_val;

        /* Extend the seen bitmap if necessary. */
        size_t new_max = (size_t)new_val;
        if (new_max > seen_cap) {
            size_t new_seen_cap = new_max;
            /* grow by at least 2× to reduce realloc次数 */
            if (new_seen_cap < seen_cap * 2)
                new_seen_cap = seen_cap * 2;
            seen = (uint8_t *)realloc(seen, (new_seen_cap + 1) * sizeof(uint8_t));
            memset(seen + seen_cap + 1, 0, (new_seen_cap - seen_cap));
            seen_cap = new_seen_cap;
        }
        seen[(size_t)new_val] = 1;

        seq_len++;
    }

    return seq[n - 1];
}
