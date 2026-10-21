/*
 * A232741  Numbers n for which the largest m such that (m-1)! divides n is a
 *          prime.
 * https://oeis.org/A232741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 221, u16 n <= 8748, u32 n <= 8748, u64 n <= 8748,
 *            u128 n <= 8748, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of sequence A232741.
   A(n) is the nth integer n such that the largest m with (m-1)! | n
   is a prime.  The function tests each integer in order, keeps a
   memoised list of the values found, and returns the requested
   term.  It uses the helper is_prime() from the utility headers. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* --------------------------------------------------------------- */
/*  Helper: factorial precomputation (up to 20! for 64‑bit builds)  */
/* --------------------------------------------------------------- */
static seq_t fact[21];
static bool fact_init = false;

static void init_factorials(void) {
    if (fact_init)
        return;
    fact[0] = (seq_t)1;
    for (size_t i = 1; i <= 20; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    fact_init = true;
}

/* --------------------------------------------------------------- */
/*  Test: does v satisfy the sequence condition?                  */
/* --------------------------------------------------------------- */
static bool good_value(seq_t v) {
    init_factorials();
    size_t last_i = 1; /* 1! always divides v */
    for (size_t i = 1; i <= 20; ++i) {
        if ((v % fact[i]) == 0)
            last_i = i;
        else
            break; /* higher factorials cannot divide v */
    }
    return is_prime((idx_t)(last_i + 1));
}

/* --------------------------------------------------------------- */
/*  Memoised sequence storage                                    */
/* --------------------------------------------------------------- */
static seq_t  *seq      = NULL; /* array of stored terms         */
static size_t seq_len   = 0;    /* number of terms stored        */
static size_t seq_cap   = 0;    /* capacity of array             */
static seq_t  next_val  = 1;    /* next integer to be tested     */

/* --------------------------------------------------------------- */
/*  External function: a(n)                                      */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset is 1, index 0 unused */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        seq_t v = next_val;
        if (good_value(v)) {
            /* Grow array if necessary */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* memory failure */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = v;
        }
        ++next_val;
    }
    return seq[n - 1];
}
