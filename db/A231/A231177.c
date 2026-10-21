/*
 * A231177  Let A = {1,4,5,8,10,11,13,...} be the sequence of numbers k>=1
 *          such that k+3 is odious (A000069), and let B be the complement of
 *          A. The sequence lists the numbers for which the number of
 *          A-divisors equals the number of B-divisors.
 * https://oeis.org/A231177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   odd_popcount_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A231177 – perfect squares whose proper divisors split
    equally between
        A = {k ≥ 1 | popcount(k + 3) is odd}
    and its complement B.
    The function returns the n‑th term (offset 1).  */
/*  The algorithm enumerates m from 1 upward, tests m²,
    counts proper divisors belonging to A and B, and stores
    those squares that satisfy cntA == cntB.  Results are memoised
    so successive calls are O(1).  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/odd_popcount_big.h"

/* -------------------------------------------------------------- */
/*  Small factorisation routine for 64‑bit integers                */
static void
factor_uint64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t d = 2; (__int128)d * d <= n; d += (d == 2) ? 1 : 2) {
        if (n % d != 0) continue;
        int e = 0;
        while (n % d == 0) { n /= d; ++e; }
        pr[c] = d;
        ex[c] = e;
        ++c;
    }
    if (n > 1) {
        pr[c]   = n;
        ex[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* -------------------------------------------------------------- */

/* -------------------------------------------------------------- */
/*  Recursively enumerate all divisors of a square.  For each      */
/*  proper divisor we increment cntA or cntB according to         */
/*  membership in A (odd popcount(div+3)) or B.                    */
static void
rec_divisors(const seq_t square,
             const uint64_t *pr, const int *ex,
             size_t fac_cnt, size_t idx,
             seq_t cur,
             uint64_t *cntA, uint64_t *cntB)
{
    if (idx == fac_cnt) {
        if (cur != square) {            /* exclude the square itself   */
            if (odd_popcount_big(cur + 3))
                ++(*cntA);
            else
                ++(*cntB);
        }
        return;
    }

    uint64_t p   = pr[idx];
    int    e    = ex[idx];
    size_t limit = (size_t)(2 * e);     /* exponent in square is 2e   */
    seq_t cur2 = cur;

    for (size_t k = 0; k <= limit; ++k) {
        rec_divisors(square, pr, ex, fac_cnt, idx + 1,
                     cur2, cntA, cntB);
        if (k < limit)
            cur2 *= (seq_t)p;
    }
}

/* -------------------------------------------------------------- */
/*  Dynamic array to store qualifying squares                     */
static seq_t *terms = NULL;      /* array of computed terms   */
static size_t terms_sz  = 0;     /* number of stored terms    */
static size_t terms_cap = 0;     /* capacity of the array    */
static uint64_t next_m   = 1;    /* next integer to test     */

/* -------------------------------------------------------------- */
/*  Generate terms until we have at least n of them                */
static void
generate_up_to(idx_t n)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms     = (seq_t *)realloc(NULL, terms_cap * sizeof(seq_t));
    }

    while (terms_sz < (size_t)n) {
        uint64_t m = next_m++;
        seq_t square = (seq_t)m * (seq_t)m;

        /* factorise m – needed for divisor enumeration of m²   */
        uint64_t fac_pr[32];
        int     fac_ex[32];
        size_t  fac_cnt;
        factor_uint64(m, fac_pr, fac_ex, &fac_cnt);

        /* count divisors of m² that belong to A or B   */
        uint64_t cntA = 0, cntB = 0;
        rec_divisors(square, fac_pr, fac_ex, fac_cnt, 0,
                     (seq_t)1, &cntA, &cntB);

        if (cntA == cntB) {            /* term qualifies       */
            if (terms_sz == terms_cap) {
                size_t new_cap = terms_cap * 2;
                terms = (seq_t *)realloc(terms,
                                         new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_sz++] = square;
        }
    }
}

/* -------------------------------------------------------------- */
/*  The public function required by the harness.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but guard anyway */
    generate_up_to(n);
    return terms[n - 1];
}
