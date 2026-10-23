/*
 * A049356  Digitally balanced numbers in base 5: equal numbers of 0's, 1's,
 *          ..., 4's.
 * https://oeis.org/A049356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049356: the nth smallest base‑5 integer whose digits
 * 0,1,2,3,4 occur in equal number of times (heterogeneous length
 * allowed).  The sequence is 1‑indexed.  Example: a(1)=10234_5 (694). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"

/* Static factorial table used by the algorithm.  It is grown lazily
 * as needed. */
static seq_t *fact      = NULL;
static size_t fact_len  = 0;
static size_t fact_cap  = 0;

/* Ensure factorial[0..needed] are computed and stored in fact[]. */
static void ensure_fact(size_t needed)
{
    if (needed >= fact_cap) {
        size_t new_cap = fact_cap ? fact_cap << 1 : 16;
        while (new_cap <= needed) new_cap <<= 1;
        seq_t *tmp = alloc_seq_array(new_cap);
        if (fact) {
            for (size_t i = 0; i < fact_len; ++i) tmp[i] = fact[i];
            /* Old block remains allocated for program lifetime. */
        }
        fact   = tmp;
        fact_cap = new_cap;
    }
    for (size_t i = fact_len; i <= needed; ++i) {
        if (i == 0) {
            fact[0] = (seq_t)1;
        } else {
            fact[i] = fact[i-1] * (seq_t)i;
        }
    }
    fact_len = needed + 1;
}

/* Number of balanced numbers of length 5*k (no leading zero). */
static seq_t count_block(size_t k)
{
    size_t len = 5 * k;
    ensure_fact(len);

    /* All permutations of k copies of each digit. */
    seq_t total = fact[len];

    seq_t denom = fact[k];
    denom = denom * denom * denom * denom * denom;      /* k!⁵ */
    seq_t all = total / denom;

    /* Subtract those with leading digit 0. */
    seq_t lead_total = fact[len - 1];
    seq_t lead_denom = fact[k - 1];
    lead_denom = lead_denom *
                 fact[k] * fact[k] * fact[k] * fact[k]; /* (k-1)! * k!⁴ */
    seq_t lead = lead_total / lead_denom;

    return all - lead;
}

/* Returns the nth term of A049356. */
seq_t a(idx_t n)
{
    /* Find block length (5*k) that contains the nth number. */
    idx_t cumulative = 0;
    size_t k = 1;
    while (true) {
        ensure_fact(5 * k);
        seq_t cnt = count_block(k);
        if (cumulative + (idx_t)cnt >= n)
            break;
        cumulative += (idx_t)cnt;
        ++k;
    }

    idx_t offset = n - cumulative - 1;   /* 0‑based within this block */

    /* Prepare digit counts (each digit appears k times). */
    unsigned count[5];
    for (int d = 0; d < 5; ++d) count[d] = (unsigned)k;

    seq_t result = 0;
    size_t L = 5 * k;    /* total number of digits */

    for (idx_t pos = 0; pos < (idx_t)L; ++pos) {
        for (int d = 0; d < 5; ++d) {
            if (pos == 0 && d == 0) continue;    /* no leading zero */
            if (count[d] == 0) continue;

            /* Try putting digit d at current position. */
            --count[d];
            idx_t rem = L - pos - 1;

            seq_t perms = fact[rem];
            for (int dd = 0; dd < 5; ++dd)
                perms /= fact[count[dd]];

            if ((idx_t)offset < perms) {
                result = result * 5 + d;
                break;   /* proceed to next position */
            } else {
                offset -= (idx_t)perms;
                ++count[d];  /* restore and try next digit */
            }
        }
    }

    return result;
}
