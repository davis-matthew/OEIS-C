/*
 * A117634  a(0)=0. a(n) = a(n-1) + 1 + (number of positive integers which are
 *          <= n and are missing from {a(0),a(1),a(2),...a(n-1)}).
 * https://oeis.org/A117634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117634
   a(0) = 0
   a(n) = a(n-1) + 1 + #{k : 1 <= k <= n and k not in {a(0),...,a(n-1)}}
   The implementation keeps a compressed presence array that grows as
   needed and recomputes missing counts for each n.  It is memoised
   so that calls can arrive in any order.  All allocations cast
   explicitly so that the code compiles under both C11 and C++17. */

#include <stdlib.h>
#include <string.h>

/* static state used for memoisation */
static seq_t *seq = NULL;           /* sequence values up to current */
static size_t seq_cap = 0;          /* allocated capacity (number of terms) */
static unsigned char *present = NULL; /* presence bitmap: present[i]==1 iff i has appeared in the sequence */
static size_t present_cap = 0;      /* allocated size of present array */
static idx_t computed = 0;          /* highest n for which seq[n] has been computed */

/* ensure present array can index up to 'needed' (inclusive) */
static void ensure_present_cap(size_t needed)
{
    if (needed < present_cap)
        return;
    size_t new_cap = present_cap ? present_cap * 2 : 8;
    while (new_cap <= needed)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(present, new_cap);
    if (tmp == NULL) abort();
    /* zero the new portion */
    memset(tmp + present_cap, 0, new_cap - present_cap);
    present = tmp;
    present_cap = new_cap;
}

/* ensure seq array can store indices up to 'needed' (inclusive) */
static void ensure_seq_cap(idx_t needed)
{
    if ((size_t)(needed + 1) <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while ((size_t)(new_cap) <= (size_t)(needed + 1))
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* lazy initialisation */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq == NULL) abort();
        seq[0] = (seq_t)0;
        present_cap = 8;
        present = (unsigned char *)malloc(present_cap * sizeof(unsigned char));
        if (present == NULL) abort();
        memset(present, 0, present_cap);
        present[0] = 1;                 /* a(0) = 0 is present but not used in missing count */
        computed = 0;
    }

    if (n > computed) {
        /* compute terms from computed+1 up to n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* make sure we can index up to i */
            ensure_present_cap((size_t)i);

            /* count how many k in [1..i] are missing */
            idx_t missing = 0;
            for (idx_t j = 1; j <= i; ++j)
                if (!present[(size_t)j])
                    ++missing;

            /* new term */
            seq_t inc = (seq_t)1 + (seq_t)missing;
            seq_t val = seq[i - 1] + inc;

            /* store result */
            ensure_seq_cap(i);
            seq[i] = val;

            /* mark this value as present for future counts */
            size_t vpos = (size_t)val;
            if (vpos >= present_cap)
                ensure_present_cap(vpos);
            present[vpos] = 1;
        }
        computed = n;
    }

    return seq[n];
}
