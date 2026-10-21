/*
 * A099520  a(1)=1; for n>1, a(n)=smallest integer m>0 not yet appearing in
 *          sequence such that m+a(n-1) is a multiple of n-1.
 * https://oeis.org/A099520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS: A099520.  
   a(1) = 1; for n>1, a(n) = smallest integer m>0 not yet appearing in the
   sequence such that m + a(n-1) is a multiple of (n-1).
*/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --- dynamic array for the sequence ------------------------------------- */
static seq_t *seq      = NULL;   /* 1‑indexed array of computed terms */
static size_t seq_len  = 0;      /* highest index that has been computed */
static size_t seq_cap  = 0;      /* current capacity of seq      */

/* ensure that seq can hold indices up to idx (inclusive) */
static void ensure_seq_cap(idx_t idx)
{
    if (idx < seq_cap) return;
    size_t cap = seq_cap ? seq_cap << 1 : 16;
    while (cap <= (size_t)idx) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, cap * sizeof(*seq));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = cap;
}

/* --- dynamic bool array that records whether a value has already been used --- */
static bool  *used      = NULL;   /* used[v] == true iff value v already appears */
static size_t used_cap  = 0;      /* capacity of used[] (indices 0 .. used_cap-1) */

/* ensure that used has room for indices up to n-1 (i.e. size n) */
static void ensure_used_cap(idx_t n)
{
    if (n <= used_cap) return;
    size_t cap = used_cap ? used_cap << 1 : 16;
    while (cap < (size_t)n) cap <<= 1;
    bool *tmp = (bool *)realloc(used, cap * sizeof(*used));
    if (!tmp) abort();
    /* initialise newly allocated slots to false */
    if (cap > used_cap) memset(tmp + used_cap, 0, (cap - used_cap) * sizeof(*used));
    used = tmp;
    used_cap = cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    /* initialise on first call */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[1] = (seq_t)1;
        seq_len = 1;
        ensure_used_cap(2);
        used[1] = true;                 /* 1 has now appeared */
    }

    /* compute missing terms up to index n */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        idx_t mod   = i - 1;                  /* divisor for congruence */
        seq_t prev  = seq[ (size_t)mod ];    /* a(n-1) */

        /* find the smallest positive m such that m + prev ≡ 0 (mod mod) */
        seq_t rem   = prev % (seq_t)mod;
        seq_t cand  = rem == 0 ? (seq_t)mod : (seq_t)mod - rem;

        /* advance cand by multiples of mod until it is unused */
        while (true) {
            ensure_used_cap( (idx_t)cand + 1 );
            if (!used[ (size_t)cand ]) break;
            cand += (seq_t)mod;
        }

        /* store the new value and mark it as used */
        ensure_seq_cap(i);
        seq[ (size_t)i ] = cand;
        seq_len = i;
        used[ (size_t)cand ] = true;
    }

    return seq[ (size_t)n ];
}
