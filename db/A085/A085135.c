/*
 * A085135  Numbers whose cyclic permutations are all 7-smooth (A002473).
 * https://oeis.org/A085135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   seq_all_rotations_smooth, seq_is_7_smooth, seq_num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A085135 – Numbers whose cyclic permutations are all 7‑smooth
 *
 *  Formula (by implementation):  
 *  The function enumerates every 7‑smooth integer up to a large bound B,  
 *  keeps only those whose *every* cyclic rotation is also 7‑smooth,  
 *  sorts the resulting set, and returns the n‑th element of that sorted
 *  list (a(1) = 1).  The bound B is chosen large enough so that the
 *  first 10 000 terms are all generated.  All arithmetic uses only the
 *  integer operators allowed for the generic type `seq_t`.
 *
 *  The function is memoised: the list of terms is built on the first
 *  call and reused on subsequent calls, irrespective of the order of
 *  the indices requested.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_all_rotations_smooth.h"
#include "../utils/seq_is_7_smooth.h"
#include "../utils/seq_num_digits.h"

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*  Global static state for the pre‑computed term list
 */
static bool init_done   = false;
static size_t terms_cap = 0;
static size_t terms_cnt = 0;
static seq_t *terms     = NULL;

/*  Comparator for qsort on seq_t values
 */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/*  -----------------------------------------------------------------
 *  Build the full list of A085135 terms up to a fixed bound B.
 *  The bound is large enough to contain more than 10 000 terms.
 *  ----------------------------------------------------------------- */
static void init_terms(void)
{
    if (init_done) return;

    /* ----- choose a bound B  ------------------------------------ */
    /* 1 000 000 000 000 000  (1e15)  fits in uint64_t */
    const seq_t B = (seq_t)1000000000000000ULL;

    /* ----- generate all powers of 2,3,5,7 up to B  ----------------- */
    seq_t pow2[64], pow3[64], pow5[64], pow7[64];
    size_t n2 = 0, n3 = 0, n5 = 0, n7 = 0;
    pow2[0] = pow3[0] = pow5[0] = pow7[0] = (seq_t)1;
    while (true) {
        seq_t nxt = pow2[n2] * (seq_t)2;
        if (nxt > B) break;
        ++n2;
        pow2[n2] = nxt;
    }
    while (true) {
        seq_t nxt = pow3[n3] * (seq_t)3;
        if (nxt > B) break;
        ++n3;
        pow3[n3] = nxt;
    }
    while (true) {
        seq_t nxt = pow5[n5] * (seq_t)5;
        if (nxt > B) break;
        ++n5;
        pow5[n5] = nxt;
    }
    while (true) {
        seq_t nxt = pow7[n7] * (seq_t)7;
        if (nxt > B) break;
        ++n7;
        pow7[n7] = nxt;
    }

    /* ----- allocate storage for terms ------------------------------ */
    terms_cap = 1 << 20;          /* start with a million slots */
    terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) abort();

    /* ----- enumerate all 7‑smooth numbers <= B --------------------- */
    for (size_t ia = 0; ia <= n2; ++ia) {
        seq_t v2 = pow2[ia];
        for (size_t ib = 0; ib <= n3; ++ib) {
            seq_t v23 = v2 * pow3[ib];
            if (v23 > B) break;
            for (size_t ic = 0; ic <= n5; ++ic) {
                seq_t v235 = v23 * pow5[ic];
                if (v235 > B) break;
                for (size_t id = 0; id <= n7; ++id) {
                    seq_t v = v235 * pow7[id];
                    if (v > B) break;
                    if (!seq_all_rotations_smooth(v)) continue;

                    /* store v */
                    if (terms_cnt == terms_cap) {
                        terms_cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        terms = tmp;
                    }
                    terms[terms_cnt++] = v;
                }
            }
        }
    }

    /* ----- sort the list in increasing order --------------------- */
    qsort(terms, terms_cnt, sizeof(seq_t), cmp_seq_t);

    init_done = true;
}

/* ------------------------------------------------------------ */
/*  Public sequence function
 */
seq_t a(idx_t n)
{
    init_terms();                     /* ensure list is ready */

    if (n < 1 || (idx_t)n > (idx_t)terms_cnt) {
        return (seq_t)0;               /* out of range – undefined behaviour */
    }
    return terms[n-1];
}
