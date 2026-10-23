/*
 * A037381  Numbers k such that every base-3 digit of k is a base-5 digit of
 *          k.
 * https://oeis.org/A037381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037381: Numbers k such that every base‑3 digit of k is a base‑5 digit of k.
   Computation is performed by scanning integers starting at 1 and keeping a static cache
   of the terms that satisfy the digit‑membership condition.  The cache is grown dynamically
   as needed, so calls to a() are independent of their order.  The algorithm runs in
   O(n) time for the n‑th term.  It uses only integer arithmetic and bit masks,
   so it works for any seq_t that supports the standard integer operators. */

#ifndef SEQ_A037381_H
#define SEQ_A037381_H

/* The function to be provided by OEIS */
seq_t a(idx_t n);

/* ------------------------------------------------------------------------- */
/* Implementation details */

static const seq_t THREE = (seq_t)3;
static const seq_t FIVE  = (seq_t)5;

/* Cached sequence values (preallocated capacity for 20 000 terms to cover n≈10 000). */
#define MAX_CACHE 20000
static seq_t seq_cache[MAX_CACHE];
static idx_t cache_size   = 0;   /* number of terms stored in seq_cache          */
static seq_t next_candidate = (seq_t)1; /* next integer to test for the property       */

/* Ensures that seq_cache contains at least 'target' terms (1‑based index). */
static void fill_cache_to(idx_t target)
{
    if (target <= cache_size) return;

    while (cache_size < target) {
        seq_t cand = next_candidate;

        /* Determine the set of digits in base‑3. */
        unsigned char mask3 = 0;
        seq_t tmp = cand;
        while (tmp != (seq_t)0) {
            unsigned d = (unsigned)(tmp % THREE);
            mask3 |= 1u << d;
            tmp = tmp / THREE;
        }

        /* Determine the set of digits in base‑5. */
        unsigned char mask5 = 0;
        tmp = cand;
        while (tmp != (seq_t)0) {
            unsigned d = (unsigned)(tmp % FIVE);
            mask5 |= 1u << d;
            tmp = tmp / FIVE;
        }

        /* Accept the candidate if every base‑3 digit occurs in the base‑5 representation. */
        if ((mask3 & ~mask5) == 0) {
            /* Cache the value.  The static array is large enough for the problem size. */
            seq_cache[cache_size++] = cand;
        }

        next_candidate = next_candidate + (seq_t)1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for offset 1, safe default */
    fill_cache_to(n);
    return seq_cache[n - 1];
}

#endif /* SEQ_A037381_H */
