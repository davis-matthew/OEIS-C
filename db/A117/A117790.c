/*
 * A117790  Lucas numbers for which the sum of the digits is a prime.
 * https://oeis.org/A117790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes terms of OEIS A117790: Lucas numbers whose digit sum is a prime.
    The first term is 1 (included by definition of the sequence), all subsequent
    Lucas numbers Lk (k ≥ 2) are considered, and those with a prime digit sum
    are added to the sequence.  The function is memoised, allowing calls
    in any order without re‑generating earlier terms. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"

static seq_t *terms   = NULL;   /* dynamic array of sequence values          */
static size_t  t_cap  = 0;      /* current capacity of the array            */
static size_t  t_sz   = 0;      /* number of terms stored                    */

static seq_t luca_prev2 = 0;    /* L(k‑2)                                    */
static seq_t luca_prev1 = 0;    /* L(k‑1)                                    */
static bool   luca_init  = false;/* whether Lucas generator has been set      */

/* Ensure that the array has at least 'need' slots. */
static void grow_terms(size_t need)
{
    if (need <= t_cap)
        return;
    size_t new_cap = t_cap ? t_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();              /* allocation failure                    */
    terms = tmp;
    t_cap = new_cap;
}

/* Initialise the Lucas generator and the terms array with the first term. */
static void init_sequence(void)
{
    if (luca_init)
        return;
    /* Lucas initial values: L0 = 2, L1 = 1.  The sequence starts with 1. */
    luca_prev2 = (seq_t)2;
    luca_prev1 = (seq_t)1;
    grow_terms(1);
    terms[0] = (seq_t)1;      /* a(1) = 1                                    */
    t_sz = 1;
    luca_init = true;
}

/* Append a new term to the cached sequence. */
static void append_term(seq_t val)
{
    grow_terms(t_sz + 1);
    terms[t_sz++] = val;
}

/*  Main function requested by the judge.  */
seq_t a(idx_t n)
{
    if (n == 0)
        abort();              /* no 0‑th term defined                      */
    init_sequence();

    /* Generate Lucas numbers until we have at least n terms. */
    while (t_sz < (size_t)n)
    {
        /* next Lucas number Lk = L(k‑1) + L(k‑2) */
        seq_t next = luca_prev2 + luca_prev1;

        /* detect unsigned overflow for fixed‑width integer types */
        if (next < luca_prev1)
            break;              /* further values cannot be represented   */

        luca_prev2 = luca_prev1;
        luca_prev1 = next;

        /* digit sum of the candidate */
        seq_t dsum = digit_sum(next);
        /* test whether that sum is prime; the sum is very small */
        if (is_prime_small((idx_t)dsum))
            append_term(next);
    }

    return terms[n - 1];
}
