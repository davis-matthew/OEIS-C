/*
 * A022943  a(n) = a(n-2) + c(n-1) for n >= 3, a( ) increasing, given a(1)=2,
 *          a(2)=3, where c( ) is complement of a( ).
 * https://oeis.org/A022943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3; for n>=3, a(n) = a(n-2) + c(n-1), where c(k) is
 *            the k‑th natural number not appearing among the sequence terms.
 *            The sequence is increasing. The implementation generates
 *            complementary numbers on the fly using a simple Boolean bitset
 *            that tracks which natural numbers have already be...
 * Range:     u8 n <= 26, u16 n <= 487, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memset */

/* --- static storage --------------------------------------------- */

static seq_t *seq      = NULL;   /* the sequence values */
static size_t seq_len  = 0, seq_cap = 0;

static seq_t *comps    = NULL;   /* complements c(1), c(2), ... */
static size_t comp_len = 0, comp_cap = 0;

static unsigned char *used   = NULL;   /* used[x]==1 ⇔ x is already in a() */
static size_t       used_cap = 0;
static size_t       next_missing = 1;  /* next candidate for the next complement */

/* --- memory helpers ------------------------------------------------ */

static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_comp_cap(size_t needed)
{
    if (needed <= comp_cap) return;
    size_t new_cap = comp_cap ? comp_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    comps = (seq_t *)realloc(comps, new_cap * sizeof(seq_t));
    comp_cap = new_cap;
}

static void ensure_used_cap(size_t needed)
{
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    used = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
    memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
    used_cap = new_cap;
}

/* --- complement generator ---------------------------------------- */

static seq_t get_next_complement(void)
{
    /* skip already used numbers */
    while (next_missing < used_cap && used[next_missing]) ++next_missing;
    seq_t val = (seq_t)next_missing;
    ++next_missing;
    return val;
}

/* --- public function ---------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but safe */

    /* initialisation on the first call */
    if (seq_len == 0) {
        ensure_seq_cap(2);
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)3;
        seq_len = 2;

        /* mark the first two terms as used */
        ensure_used_cap(4);          /* cover indices 0..3 at least */
        used[(size_t)2] = 1;
        used[(size_t)3] = 1;
    }

    /* extend precomputation up to the requested index */
    if ((idx_t)seq_len < n) {
        size_t need = (size_t)n;
        ensure_seq_cap(need);
        ensure_comp_cap(need > 1 ? need - 1 : 0);   /* need - 1 complements */

        for (size_t i = seq_len + 1; i <= need; ++i) {
            seq_t ai;

            if (i == 1) {
                ai = (seq_t)2;
            } else if (i == 2) {
                ai = (seq_t)3;
            } else {
                /* make sure the (i-2)-th complement has been generated */
                while (comp_len <= i - 2) {
                    comps[comp_len++] = get_next_complement();
                }
                ai = seq[i - 3] + comps[i - 2];
            }

            /* store the new term */
            seq[seq_len++] = ai;

            /* mark the new value as used */
            size_t idx = (size_t)ai;
            ensure_used_cap(idx + 1);
            used[idx] = 1;
        }
    }

    return seq[n - 1];
}
