/*
 * A214371  If a(n) has not yet been defined then set a(n) = least positive
 *          integer that has not yet occurred; also if n>1 and a(n+a(n)) has
 *          not yet been defined then set a(n+a(n)) = a(n).
 * https://oeis.org/A214371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1. For n>1, • if a(n) has not yet been defined, set a(n)
 *            to the least positive integer that has not yet occurred, • if
 *            n>1 and a(n+a(n)) has not yet been defined, set a(n+a(n)) =
 *            a(n). This is computed by memoising values and propagating the
 *            second rule only when n > 1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* avoid requiring external <limits.h> for UINT64_MAX */
#ifndef UINT64_MAX
# define UINT64_MAX 0xFFFFFFFFFFFFFFFFULL
#endif

/* ------------------------------------------------------------ */
/* dynamic arrays for the sequence and the used flag array      */
/* ------------------------------------------------------------ */

static seq_t *a_vals = NULL;            /* 1‑index array of the sequence */
static size_t a_cap = 0;                /* current capacity in elements    */
static uint8_t *used_flags = NULL;      /* 0 = unused, 1 = used            */
static size_t used_cap = 0;             /* current capacity for flags      */
static idx_t processed = 0;             /* maximum index that has been filled */
static idx_t next_candidate = 0;        /* next integer to test for unused */

/* ensure a_vals has room for index need (inclusive) */
static void ensure_seq_cap(idx_t need)
{
    if (need < (idx_t)a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 64;
    while (new_cap <= (size_t)need) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!newp) abort();
    if (new_cap > a_cap) {
        memset(newp + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
    }
    a_vals = newp;
    a_cap = new_cap;
}

/* ensure used_flags has room for value need (inclusive) */
static void ensure_used_cap(idx_t need)
{
    if (need < (idx_t)used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap <= (size_t)need) new_cap *= 2;
    uint8_t *newp = (uint8_t *)realloc(used_flags, new_cap * sizeof(uint8_t));
    if (!newp) abort();
    if (new_cap > used_cap) {
        memset(newp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    }
    used_flags = newp;
    used_cap = new_cap;
}

/* find the smallest positive integer not yet used */
static seq_t find_next_unused(void)
{
    do {
        ++next_candidate;
        ensure_used_cap(next_candidate);
    } while (used_flags[(size_t)next_candidate]);
    return (seq_t)next_candidate;
}

/* ------------------------------------------------------------ */
/* sequence function                                           */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n==0, fall back */

    /* make sure the array can hold index n */
    ensure_seq_cap(n);

    /* fill missing terms up to n */
    for (idx_t i = processed + 1; i <= n; ++i)
    {
        /* rule 1: assign a(i) if not yet defined */
        if (a_vals[i] == 0) {
            seq_t val = find_next_unused();
            a_vals[i] = val;
            ensure_used_cap((idx_t)val);
            used_flags[(size_t)val] = 1;
        }

        /* rule 2: propagate only when i > 1 */
        if (i > 1) {
            __uint128_t j128 = ( __uint128_t )i + ( __uint128_t )a_vals[i];
            if (j128 <= ( __uint128_t )UINT64_MAX) {
                idx_t j = (idx_t)j128;
                ensure_seq_cap(j);
                if (a_vals[j] == 0) {
                    a_vals[j] = a_vals[i];
                }
            }
        }
    }

    processed = n;
    return a_vals[n];
}
