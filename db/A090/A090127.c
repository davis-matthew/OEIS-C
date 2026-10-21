/*
 * A090127  Distinct values of phi(n) in order of appearance as n increases.
 * https://oeis.org/A090127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A210719(n))
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/phi.h"

static seq_t *distinct_vals = NULL;
static size_t distinct_len  = 0;
static size_t distinct_cap  = 0;
static idx_t  last_m_processed = 0;

/* Ensure at least 'needed' slots in distinct_vals. */
static void ensure_distinct_capacity(size_t needed)
{
    if (needed <= distinct_cap) return;
    size_t newcap = distinct_cap ? distinct_cap * 2 : 64;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(distinct_vals, newcap * sizeof(seq_t));
    if (!tmp) abort();          /* memory exhaustion */
    distinct_vals = tmp;
    distinct_cap  = newcap;
}

/* Append a new distinct φ value. */
static void add_distinct(seq_t val)
{
    ensure_distinct_capacity(distinct_len + 1);
    distinct_vals[distinct_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; behaviour for 0 is irrelevant */

    /* If we have already cached enough values, return directly. */
    if (distinct_len >= (size_t)n)
        return distinct_vals[(size_t)n - 1];

    /* Generate further φ values until we have at least n distinct ones. */
    while (distinct_len < (size_t)n) {
        ++last_m_processed;                       /* next m to evaluate */
        seq_t phi_val = phi(last_m_processed);    /* φ(m) */

        /* Check whether phi_val is already in the list. */
        bool duplicate = false;
        for (size_t i = 0; i < distinct_len; ++i) {
            if (distinct_vals[i] == phi_val) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate)
            add_distinct(phi_val);
    }

    return distinct_vals[(size_t)n - 1];
}
