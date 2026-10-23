/*
 * A138153  If the numbers a(1)...a(n) contain a hole, then a(n+1) is the
 *          smallest hole; otherwise a(n+1) = a(n-2) + a(n-1) + a(n).
 * https://oeis.org/A138153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A138153 – "Holes and Tribonacci"
 *
 *  Initial terms: a(1)=1, a(2)=2, a(3)=3.
 *
 *  For n ≥ 4 let H be the set of positive integers less than the current
 *  maximum a(k) that have not appeared among a(1)…a(k‑1).  If H is non‑empty
 *  then   a(n) = min(H)   (fill the smallest hole);
 *  otherwise   a(n) = a(n‑3) + a(n‑2) + a(n‑1)   (tribonacci).
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* global static storage for memoisation                               */
/* ------------------------------------------------------------------ */
static seq_t *seq_buf       = NULL;   /* stored sequence values            */
static size_t seq_cap       = 0;      /* allocated capacity of seq_buf     */
static idx_t  seq_len       = 0;      /* number of values currently stored */

static seq_t *used_buf      = NULL;   /* list of values that have appeared */
static size_t used_cap      = 0;      /* allocated capacity of used_buf    */
static size_t used_len      = 0;      /* number of values currently used   */

static seq_t min_missing     = 1;     /* smallest positive integer not yet used */
static seq_t max_used        = 0;     /* current maximum value seen         */

/* ------------------------------------------------------------------ */
/* helper: test whether a value has already appeared                  */
/* ------------------------------------------------------------------ */
static bool used_contains(seq_t v)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_buf[i] == v)
            return true;
    return false;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The sequence is indexed from 1; we store elements in 0‑based array */
    if (n == 0) {
        /* undefined for the purpose of the OEIS offset; return 0 to silence warnings */
        return (seq_t)0;
    }

    /* initialise storage once */
    if (!seq_buf) {
        seq_cap   = 32;
        seq_buf   = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        used_cap  = 32;
        used_buf  = (seq_t *)malloc(used_cap * sizeof(seq_t));
        seq_len   = 0;
        used_len  = 0;
        min_missing = 1;
        max_used    = 0;
    }

    /* generate terms until we reach the requested index */
    while (seq_len < n) {

        seq_t next = 0;

        if (seq_len == 0) {                    /* a(1) */
            next = 1;
        } else if (seq_len == 1) {             /* a(2) */
            next = 2;
        } else if (seq_len == 2) {             /* a(3) */
            next = 3;
        } else {
            if (min_missing <= max_used) {
                next = min_missing;            /* fill a hole */
            } else {                           /* tribonacci   */
                size_t idx = (size_t)(seq_len - 3);
                next = seq_buf[idx] + seq_buf[idx + 1] + seq_buf[idx + 2];
            }
        }

        /* store the new term in the sequence buffer */
        if (seq_len >= seq_cap) {
            seq_cap = seq_cap * 2;
            seq_buf = (seq_t *)realloc(seq_buf, seq_cap * sizeof(seq_t));
        }
        seq_buf[seq_len] = next;

        /* keep a record of all used values */
        if (used_len >= used_cap) {
            used_cap = used_cap * 2;
            used_buf = (seq_t *)realloc(used_buf, used_cap * sizeof(seq_t));
        }
        used_buf[used_len++] = next;

        /* update the running maximum if necessary */
        if (next > max_used)
            max_used = next;

        /* advance min_missing if we just used that value */
        if (next == min_missing) {
            do {
                ++min_missing;
            } while (used_contains(min_missing));
        }

        ++seq_len;
    }

    return seq_buf[(size_t)(n - 1)];
}
