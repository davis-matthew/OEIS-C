/*
 * A139124  Starting from 1, at any step count the appearances of k, where k
 *          ranges from the highest number to 1.
 * https://oeis.org/A139124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(0)=1; for each step, assume the largest value seen so far is M.  
   Append, in descending order from M to 1, the counts of each integer present
   in the current sequence.  The sequence is built incrementally, memoising
   all terms, so a(n) can be requested in any order. */

#include <stdlib.h>

/* ---------- Static storage and helper functions ---------- */

static seq_t *seq      = NULL;          /* array of sequence values          */
static size_t seq_cap  = 0;             /* capacity of seq array             */
static size_t seq_len  = 0;             /* number of terms currently stored  */

static seq_t *freq     = NULL;          /* frequency of each integer value   */
static size_t freq_cap = 0;             /* capacity of freq array            */

static idx_t max_idx   = 1;             /* current maximum integer value seen */

/* Ensure the seq array can hold at least 'needed' elements. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < seq_len; ++i) tmp[i] = seq[i];
    delete[] seq; seq = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    /* In a real program you should check tmp==NULL. */
    seq = tmp;
#endif
    seq_cap = new_cap;
}

/* Ensure the freq array can index up to 'idx'. */
static void ensure_freq_cap(idx_t idx)
{
    if ((size_t)idx < freq_cap) return;
    size_t new_cap = freq_cap ? freq_cap * 2 : 4;
    while (new_cap <= (size_t)idx) new_cap <<= 1;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < freq_cap; ++i) tmp[i] = freq[i];
    for (size_t i = freq_cap; i < new_cap; ++i) tmp[i] = 0;
    delete[] freq; freq = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(freq, new_cap * sizeof(seq_t));
    for (size_t i = freq_cap; i < new_cap; ++i) tmp[i] = 0;
    freq = tmp;
#endif
    freq_cap = new_cap;
}

/* Initialise data structures on first call. */
static void init_sequence(void)
{
    if (seq != NULL) return;            /* already initialised */

    seq_cap = 8;
    seq_len = 0;
#ifdef __cplusplus
    seq = new seq_t[seq_cap];
#else
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
#endif

    freq_cap = 4;
#ifdef __cplusplus
    freq = new seq_t[freq_cap];
#else
    freq = (seq_t *)malloc(freq_cap * sizeof(seq_t));
#endif
    for (size_t i = 0; i < freq_cap; ++i) freq[i] = 0;

    /* first term */
    seq[0] = (seq_t)1;
    seq_len = 1;
    ensure_freq_cap(1);
    freq[1] = (seq_t)1;
    max_idx = 1;
}

/* ---------- Main sequence access function ---------- */

seq_t a(idx_t n)
{
    init_sequence();

    size_t target = (size_t)n;

    /* Generate terms until we reach the desired index */
    while (seq_len <= target) {
        size_t cur_max = (size_t)max_idx;

        /* Allocate temporary storage for counts in descending order */
#ifdef __cplusplus
        seq_t *tmp_counts = new seq_t[cur_max];
#else
        seq_t *tmp_counts = (seq_t *)malloc(cur_max * sizeof(seq_t));
#endif
        size_t p = 0;
        for (idx_t k = max_idx; k >= (idx_t)1; --k) {
            tmp_counts[p++] = freq[(size_t)k];
            if (k == 1) break;          /* avoid underflow for unsigned k */
        }

        /* Append the counts and update frequencies */
        size_t new_needed = seq_len + cur_max;
        ensure_seq_cap(new_needed);
        for (size_t i = 0; i < cur_max; ++i) {
            seq_t val = tmp_counts[i];
            seq[seq_len++] = val;

            idx_t val_idx = (idx_t)val;   /* counts are small enough for idx_t */
            ensure_freq_cap(val_idx);
            freq[val_idx]++;              /* update frequency for next step */

            if (val_idx > max_idx) max_idx = val_idx;
        }

        /* Free temporary array */
#ifndef __cplusplus
        free(tmp_counts);
#else
        delete[] tmp_counts;
#endif
    }

    return seq[target];
}
