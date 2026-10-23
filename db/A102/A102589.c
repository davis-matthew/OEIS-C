/*
 * A102589  Numbers of the forms 4*p or q where both numbers p=(10^n-7)/3 and
 *          q=(127*10^(n-1)-7)/3 are primes.
 * https://oeis.org/A102589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A102589
   This function generates the subsequence of numbers that are either 4*p
   or q where both
        p = (10^n - 7)/3      and
        q = (127*10^(n-1) - 7)/3
   are prime.  For each admissible n (beginning at n = 2) the two
   numbers 4*p and q are emitted in that order.  The sequence is
   defined by the recurrence
        p_{k+1} = 10*p_k + 21
        q_{k+1} = 10*q_k + 21
   with initial values p_2 = 31 and q_2 = 421.
   The function builds the list on demand and memoises the results
   so that calls may arrive in arbitrary order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t *terms = NULL;          /* cached sequence terms            */
static idx_t terms_size = 0;         /* number of cached terms           */
static idx_t terms_capacity = 0;     /* allocated capacity in terms array*/
static bool initialized = false;     /* flag whether initialisation done */
static seq_t p = 0;                  /* current p for next n             */
static seq_t q = 0;                  /* current q for next n             */

/* initialise the generator */
static void init(void)
{
    if (initialized) return;
    initialized = true;
    p = (seq_t)31;          /* p for n = 2 */
    q = (seq_t)421;         /* q for n = 2 */
}

/* ensure the terms array can hold at least new_cap entries */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= terms_capacity) return;
    idx_t cap = terms_capacity ? terms_capacity << 1 : 4;
    if (cap < new_cap) cap = new_cap;
    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
    terms = tmp;
    terms_capacity = cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init();                           /* ensure p and q are set          */
    while (terms_size < n) {          /* generate terms until enough     */
        if (is_prime_seq(p) && is_prime_seq(q)) {
            /* emit the two terms 4*p and q */
            ensure_capacity(terms_size + 2);
            terms[terms_size++] = (seq_t)4 * p;
            terms[terms_size++] = q;
        }
        /* advance to next n (p_{k+1} = 10*p_k + 21, same for q) */
        p = (seq_t)10 * p + (seq_t)21;
        q = (seq_t)10 * q + (seq_t)21;
    }
    /* return the requested entry (1‑based index) */
    return terms[n - 1];
}
