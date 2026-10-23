/*
 * A135592  a(1)=1; for n > 1, a(n) is number of earlier terms equal to number
 *          of prime divisors of n.
 * https://oeis.org/A135592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(1)=1; for n>1, a(n)=#{k < n : a(k)=ω(n)},
   where ω(n) is the number of distinct prime divisors of n. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_omega.h"

/* ---- static caches for the sequence ----------------------- */
static seq_t *seq_cache = NULL;   /* holds a(k) for k = 1 .. seq_len */
static size_t seq_cap  = 0;       /* allocation capacity of seq_cache */
static size_t seq_len  = 0;       /* number of elements already stored */

/* ---- frequency table: freq[v] = number of occurrences of value v
          among already computed terms ----------------------------------- */
static size_t *freq = NULL;      /* freq array is indexed by value v  */
static size_t freq_cap = 0;      /* capacity of freq array             */

/* initialize caches, called once */
static void init_caches(void) {
    if (seq_cache != NULL) return;          /* already initialized */

    /* sequence cache: at least one slot for a(1) */
    seq_cap = 1;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_cache[0] = (seq_t)1;                /* a(1) = 1 */
    seq_len = 1;

    /* frequency table: need index 0 and 1 at least */
    freq_cap = 2;
    freq = (size_t *)malloc(freq_cap * sizeof(size_t));
    for (size_t i = 0; i < freq_cap; ++i) freq[i] = 0;
    freq[1] = 1;                            /* one occurrence of value 1 so far */
}

/* ensure seq_cache can hold at least 'need' elements */
static void grow_seq_cache(size_t need) {
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure freq array can index up to 'need' (exclusive) */
static void grow_freq(size_t need) {
    if (freq_cap > need) return;
    size_t new_cap = freq_cap ? freq_cap * 2 : 1;
    while (new_cap <= need) new_cap <<= 1;
    size_t *newfreq = (size_t *)realloc(freq, new_cap * sizeof(size_t));
    /* initialise new entries to zero */
    for (size_t i = freq_cap; i < new_cap; ++i) newfreq[i] = 0;
    freq = newfreq;
    freq_cap = new_cap;
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    init_caches();

    size_t up_to = (size_t)n;
    for (size_t cur = seq_len + 1; cur <= up_to; ++cur) {
        /* compute ω(cur) = number of distinct prime divisors */
        unsigned int omega = count_omega((uint64_t)cur);

        /* current term is the number of previous terms equal to ω(cur) */
        seq_t val = (omega < freq_cap) ? (seq_t)freq[omega] : (seq_t)0;

        /* store the new term */
        grow_seq_cache(seq_len + 1);
        seq_cache[seq_len] = val;
        ++seq_len;

        /* update frequency table for this value */
        size_t val_idx = (size_t)val;
        grow_freq(val_idx);
        freq[val_idx] += 1;
    }

    return seq_cache[(size_t)n - 1];
}
