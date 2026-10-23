/*
 * A022951  a(n) = a(n-1) + c(n) for n >= 3, a( ) increasing, given a(1)=2,
 *          a(2)=3; where c( ) is complement of a( ).
 * https://oeis.org/A022951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n−1) + c(n) for n ≥ 3, a(1)=2, a(2)=3; c(n) is the n'th
 *            positive integer that is not in the sequence. The sequence is
 *            identical to OEIS A022947
 * Range:     u8 n <= 19, u16 n <= 346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   raw_free, raw_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/raw_free.h"
#include "../utils/raw_malloc.h"

/* Static storage for the sequence values */
static seq_t *a_vals = NULL;   /* array of computed a(n) values          */
static size_t a_cap  = 0;      /* current allocated capacity             */
static size_t a_len  = 0;      /* number of values stored (sequence length)*/

/* Static storage for the used numbers (bitset)  */
static uint64_t *used_bits     = NULL;   /* bit i set → i is used               */
static idx_t    used_cap_bits = 0;        /* number of bits currently allocated */

/* Current state of the complement search  */
static idx_t cur_x = 0;        /* next integer to test for complement  */
static idx_t found_comps = 0;  /* how many complements already found   */

#ifdef __cplusplus

#else

#endif

/*-----------------------------------------------------------*/
/* Bitset operations                                         */
static inline bool is_used(idx_t v)
{
    return (used_bits[v >> 6] & (1ULL << (v & 63))) != 0;
}

static inline void mark_used(idx_t v)
{
    used_bits[v >> 6] |= (1ULL << (v & 63));
}

static void ensure_used_capacity(idx_t bits_needed)
{
    if (used_cap_bits >= bits_needed) return;
    idx_t new_bits = used_cap_bits ? used_cap_bits : 64;
    while (new_bits < bits_needed) new_bits <<= 1;

    size_t new_words = (new_bits + 63) >> 6;
    size_t old_words = (used_cap_bits + 63) >> 6;

    uint64_t *tmp = (uint64_t *)raw_malloc(new_words, sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    if (old_words > 0) memcpy(tmp, used_bits, old_words * sizeof(uint64_t));
    if (new_words > old_words)
        memset(tmp + old_words, 0, (new_words - old_words) * sizeof(uint64_t));

    if (used_bits) raw_free(used_bits);
    used_bits = tmp;
    used_cap_bits = new_bits;
}

/*------------------------------------------------------------*/
/* Capacity handling for the sequence array                  */
static void ensure_seq_capacity(size_t needed)
{
    if (a_cap >= needed) return;
    size_t new_cap = a_cap ? (a_cap << 1) : 4;
    while (new_cap < needed) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    /* Copy existing elements */
    for (size_t i = 0; i < a_len; ++i) {
        new (&tmp[i]) seq_t(a_vals[i]);   /* placement new */
    }
    if (a_vals) delete[] a_vals;
    a_vals = tmp;
#else
    seq_t *tmp = (seq_t *)raw_malloc(new_cap, sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    if (a_vals) {
        memcpy(tmp, a_vals, a_len * sizeof(seq_t));
        raw_free(a_vals);
    }
    a_vals = tmp;
#endif
    a_cap = new_cap;
}

/*------------------------------------------------------------*/
/* Initialization of the sequence and bitset                 */
static void init_sequence(void)
{
    if (a_len > 0) return;          /* already initialised */

    ensure_seq_capacity(4);
    a_vals[0] = (seq_t)2;
    a_vals[1] = (seq_t)3;
    a_len    = 2;

    /* Bitset for used numbers */
    ensure_used_capacity(4);
    mark_used((idx_t)2);
    mark_used((idx_t)3);

    /* Start complement search at 0 */
    cur_x        = 0;
    found_comps  = 0;
}

/*------------------------------------------------------------*/
/* Extend the sequence up to index n (1‑based)                */
static void extend_to(idx_t n)
{
    /* indices in a_vals are 0‑based, so a_len = current length */
    for (idx_t i = a_len + 1; i <= n; ++i) {
        /* Find the i‑th complement */
        idx_t comp_val = 0;
        while (found_comps < i) {
            ++cur_x;
            if (cur_x >= used_cap_bits)
                ensure_used_capacity((idx_t)cur_x + 1);
            if (!is_used(cur_x)) {
                ++found_comps;
                if (found_comps == i)
                    comp_val = cur_x;
            }
        }

        /* Compute next sequence value: a(i) = a(i−1) + comp_val */
        seq_t prev = a_vals[a_len - 1];
        seq_t next = prev + (seq_t)comp_val;

        /* Store it */
        ensure_seq_capacity(a_len + 1);
        a_vals[a_len++] = next;

        /* Mark this number as used for future searches */
        if (next >= (seq_t)used_cap_bits)
            ensure_used_capacity((idx_t)next + 1);
        mark_used((idx_t)next);
    }
}

/*------------------------------------------------------------*/
/* Public function                                            */
seq_t a(idx_t n)
{
    init_sequence();
    if (n <= a_len)
        return a_vals[n - 1];
    extend_to(n);
    return a_vals[n - 1];
}
