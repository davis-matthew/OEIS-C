/*
 * A138683  a(1)=1 and, for n>1, a(n) is the smallest integer greater than
 *          a(n-1) such that a(n) and a(k) do not sum to a term of A001333
 *          (Numerators of continued fraction convergents to sqrt(2)).
 * https://oeis.org/A138683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138683:
 * a(1)=2;
 * for n>1, a(n) is the smallest integer > a(n-1) such that
 * a(n)+a(k) does NOT equal any term of A001333
 * (the numerators of the convergents to √2, i.e. Pell numbers).
 * The code builds the sequence on demand, caching results for future calls.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Helpers: dynamic arrays for seq_t values                              */
/* --------------------------------------------------------------------- */

static size_t seq_cap = 0;
static size_t seq_len = 0;
static seq_t *seq = NULL;

static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) return; /* allocation failure – caller will see zero */
    if (seq) {
        memcpy(newp, seq, seq_len * sizeof(seq_t));
        free(seq);
    }
    seq = newp;
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Pell numbers (A001333) cache                                         */
/* --------------------------------------------------------------------- */
static size_t pell_cap = 0;
static size_t pell_len = 0;
static seq_t *pell = NULL;

static void ensure_pell_cap(size_t needed)
{
    if (needed <= pell_cap) return;
    size_t new_cap = pell_cap ? pell_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) return;
    if (pell) {
        memcpy(newp, pell, pell_len * sizeof(seq_t));
        free(pell);
    }
    pell = newp;
    pell_cap = new_cap;
}

/* Recursive generation of Pell numbers: P0=1, P1=3, P_{n+1}=2*P_n+P_{n-1} */
static bool is_pell_term(seq_t val)
{
    /* init first two terms if necessary */
    if (pell_len == 0) {
        ensure_pell_cap(4);
        pell[0] = (seq_t)1;
        pell[1] = (seq_t)3;
        pell_len = 2;
    }
    /* generate until last term >= val */
    while (pell_len < pell_cap && pell[pell_len - 1] < val) {
        seq_t next = (seq_t)2 * pell[pell_len - 1] + pell[pell_len - 2];
        ensure_pell_cap(pell_len + 1);
        pell[pell_len] = next;
        ++pell_len;
        /* protect against overflow in 64-bit builds */
        if (next < pell[pell_len - 2]) break;
    }
    /* linear scan – Pell list is very short for small values */
    for (size_t i = 0; i < pell_len; ++i) {
        if (pell[i] == val) return true;
        if (pell[i] > val) break;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Main sequence computation                                            */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined – not used */

    /* initialise first value if needed */
    if (seq_len == 0) {
        ensure_seq_cap(16);
        seq[0] = (seq_t)2;      /* a(1) = 2 */
        seq_len = 1;
    }

    /* extend sequence up to index n */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        seq_t candidate = prev + (seq_t)1;

        /* find smallest candidate that keeps all sums safe */
        while (1) {
            bool bad = false;
            for (size_t k = 0; k < seq_len; ++k) {
                seq_t sum = candidate + seq[k];
                if (is_pell_term(sum)) {
                    bad = true;
                    break;
                }
            }
            if (!bad) break;
            ++candidate;
        }

        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = candidate;
        ++seq_len;
    }

    return seq[n - 1];
}
