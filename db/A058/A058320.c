/*
 * A058320  Distinct even prime-gap lengths (number of composites between
 *          primes), from 3+2, 7+4, 23+6,...
 * https://oeis.org/A058320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  hard, nice, nonn
 * Formula:   a(n) = the n-th distinct even prime gap (number of composites
 *            between consecutive primes)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Adopted from the OEIS definition of A058320.  The function builds the list of primes on
 *  demand and records each new even gap that has not appeared before.  The sequence is
 *  0‑indexed: a(0)=2, a(1)=4, … as listed in the OEIS entry. */
 
#include <stdlib.h>
#include <stdbool.h>

static idx_t *primes   = NULL;           /* list of discovered primes            */
static size_t  primes_cap = 0, primes_len = 0;

static seq_t *gap_list   = NULL;           /* distinct even gap values seen so far */
static size_t  gap_cap = 0, gap_len = 0;

static seq_t *seen   = NULL;           /* auxiliary table for duplicate test  */
static size_t  seen_cap = 0, seen_len = 0;

/*--- dynamic array helpers ----------------------------------------*/
static void grow_primes(void) {
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 64;
        idx_t *p = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!p) exit(1);
        primes = p;
        primes_cap = new_cap;
    }
}
static void grow_gap_list(void) {
    if (gap_len >= gap_cap) {
        size_t new_cap = gap_cap ? gap_cap * 2 : 32;
        seq_t *g = (seq_t *)realloc(gap_list, new_cap * sizeof(seq_t));
        if (!g) exit(1);
        gap_list = g;
        gap_cap = new_cap;
    }
}
static void grow_seen(void) {
    if (seen_len >= seen_cap) {
        size_t new_cap = seen_cap ? seen_cap * 2 : 32;
        seq_t *s = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
        if (!s) exit(1);
        seen = s;
        seen_cap = new_cap;
    }
}

/*--- duplicate detection -------------------------------------------*/
static bool seen_add(seq_t val) {
    for (size_t i = 0; i < seen_len; ++i)
        if (seen[i] == val)
            return true;            /* already present */
    grow_seen();
    seen[seen_len++] = val;
    return false;                   /* first occurrence */
}

/*--- prime generation utilities ------------------------------------*/
static idx_t next_prime_after(idx_t last) {
    idx_t cand = last + 1;
    if (cand <= 2) cand = 2;
    if (cand > 2 && (cand & 1) == 0) cand++;   /* make odd if >2 */
    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            idx_t p = primes[i];
            if (p > cand / p) break;            /* p*p > cand */
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) return cand;
        cand += 2;                   /* skip even numbers */
    }
}

/*--------------------------------------------------------------------*/
seq_t a(idx_t n) {
    /* produce at least (n+1) distinct even gaps */
    while (gap_len <= n) {
        /* initialise prime list with first prime 2 */
        if (primes_len == 0) {
            grow_primes();
            primes[primes_len++] = 2;
            continue;                        /* next iteration will compute the first gap */
        }

        idx_t prev   = primes[primes_len - 1];
        idx_t nextp  = next_prime_after(prev);

        grow_primes();
        primes[primes_len++] = nextp;

        idx_t gap_int = nextp - prev;         /* gap fits in idx_t (uint64_t) */
        if ((gap_int & 1) == 0) {             /* only even gaps */
            seq_t gap = (seq_t)gap_int;
            if (!seen_add(gap)) {             /* first occurrence */
                grow_gap_list();
                gap_list[gap_len++] = gap;
            }
        }
    }
    return gap_list[n];
}
