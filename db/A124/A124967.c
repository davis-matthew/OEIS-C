/*
 * A124967  Numbers which can be expressed as the ordered sum of 3 squares in
 *          3 or more different ways.
 * https://oeis.org/A124967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124967
   Numbers that can be expressed as the sum of three squares in at least
   3 unordered ways (allowing a,b,c≥0).  a(1)=41.  This implementation
   builds a table of counts of unordered triples (a≥b≥c≥0) with
   a²+b²+c²≤limit and collects the numbers occurring at least thrice.
   The table is enlarged by doubling the limit until the requested
   amount of terms has been produced.  All operations use only integer
   types and standard library functions. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- static state ----------------------------------------------------- */
static idx_t cur_limit  = 0;          /* current sieve upper bound          */
static size_t term_count = 0;         /* number of terms found so far       */
static seq_t *terms = NULL;           /* array of found terms               */
static bool ready = false;            /* true after a successful build      */

/* ---- helper: integer square root for idx_t -------------------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((uint64_t)(r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* ---- rebuild the sieve and term table up to new_limit ------------------- */
static void rebuild(idx_t new_limit)
{
    /* allocate temporary count array */
    uint16_t *cnt = (uint16_t *)calloc((size_t)new_limit + 1, sizeof(uint16_t));
    if (!cnt) return;                          /* allocation failure */

    /* enumerating unordered triples (a≥b≥c≥0) */
    idx_t maxA = isqrt_u64(new_limit);
    for (idx_t a = 0; a <= maxA; ++a) {
        seq_t a2 = (seq_t)a * (seq_t)a;
        if (a2 > new_limit) break;
        idx_t maxB = a;
        for (idx_t b = 0; b <= maxB; ++b) {
            seq_t b2 = (seq_t)b * (seq_t)b;
            seq_t sum_ab = a2 + b2;
            if (sum_ab > new_limit) break;
            idx_t maxC = b;
            for (idx_t c = 0; c <= maxC; ++c) {
                seq_t c2 = (seq_t)c * (seq_t)c;
                seq_t s = sum_ab + c2;
                if (s > new_limit) break;
                ++cnt[(size_t)s];
            }
        }
    }

    /* first pass: count how many numbers satisfy the condition */
    size_t new_term_count = 0;
    for (idx_t i = 0; i <= new_limit; ++i)
        if (cnt[(size_t)i] >= 3) ++new_term_count;

    /* allocate or reallocate terms array */
    seq_t *new_terms = (seq_t *)realloc(terms, new_term_count * sizeof(seq_t));
    if (!new_terms) {        /* allocation failure: clean up and exit */
        free(cnt);
        return;
    }
    terms = new_terms;

    /* second pass: store the terms in ascending order */
    size_t idx = 0;
    for (idx_t i = 0; i <= new_limit; ++i) {
        if (cnt[(size_t)i] >= 3) {
            terms[idx++] = (seq_t)i;
        }
    }

    /* cleanup and store metadata */
    free(cnt);
    term_count = new_term_count;
    cur_limit   = new_limit;
    ready       = true;
}

/* ---- extend data until at least `need` terms are available ---------------- */
static void build_terms(idx_t need)
{
    if (ready && term_count >= need) return;

    /* start with an initial limit if nothing built yet */
    if (!ready) {
        rebuild(100000);          /* arbitrary starting limit */
    }

    /* double the limit until enough terms are found */
    while (term_count < need) {
        idx_t new_limit = cur_limit * 2;
        rebuild(new_limit);
    }
}

/* ---- public interface ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, safety only */
    build_terms(n);
    return terms[n - 1];
}
