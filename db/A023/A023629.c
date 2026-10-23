/*
 * A023629  a(n) = c([ n/2 ]) + n, with a(1) = 1, c = complement to a.
 * https://oeis.org/A023629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c([n/2]) + n with a(1)=1 and c = complement of a
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Forward declarations of helper functions */
static void ensure_used_cap(idx_t new_cap);
static void ensure_comp_len(idx_t k);
static void ensure_a_len(idx_t n);

/* Static state for memoisation */
static seq_t *a_seq = NULL;          /* a_seq[i] contains a(i), 1‑based */
static idx_t a_cap = 0, a_len = 0;   /* capacity and current length of a_seq */

static seq_t *comp_seq = NULL;       /* comp_seq[k] contains c(k) (1‑based) */
static idx_t comp_cap = 0, comp_len = 0;

static uint8_t *used_flag = NULL;    /* flags for numbers that appear in a */
static idx_t used_cap = 0;           /* capacity of used_flag array */

static idx_t next_candidate = 1;     /* next integer to test for being missing */

/* Ensure used_flag has capacity at least new_cap (indices 0 … new_cap-1) */
static void ensure_used_cap(idx_t new_cap)
{
    if (new_cap <= used_cap)
        return;
    size_t new_sz = (size_t)new_cap;
    uint8_t *tmp = (uint8_t *)realloc(used_flag, new_sz);
    if (!tmp)
        exit(1);   /* allocation failure */
    if (new_sz > used_cap)
        memset(tmp + used_cap, 0, new_sz - used_cap);
    used_flag = tmp;
    used_cap = new_cap;
}

/* Extend comp_seq so that it contains at least the first k missing numbers
   (i.e. the complement of the set of values in a_seq). */
static void ensure_comp_len(idx_t k)
{
    while (comp_len < k) {
        /* Find next missing candidate */
        while (true) {
            if (next_candidate >= used_cap)
                ensure_used_cap(next_candidate + 1);
            if (!used_flag[(idx_t)next_candidate]) {
                /* It's missing; record it as the next c(k) */
                comp_len++;
                if (comp_len > comp_cap) {
                    idx_t new_cap = comp_cap ? comp_cap * 2 : 2;
                    if (new_cap < comp_len)
                        new_cap = comp_len;
                    seq_t *new_arr = (seq_t *)realloc(comp_seq, (new_cap + 1) * sizeof(seq_t));
                    if (!new_arr)
                        exit(1);
                    comp_seq = new_arr;
                    comp_cap = new_cap;
                }
                comp_seq[comp_len] = next_candidate;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
}

/* Generate a(n) up to at least n, extending the memoised arrays as needed. */
static void ensure_a_len(idx_t n)
{
    while (a_len < n) {
        a_len++;
        idx_t i = a_len;
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else {
            idx_t k = i / 2;           /* floor(i/2) */
            ensure_comp_len(k);
            val = comp_seq[k] + (seq_t)i;
        }

        /* Mark this value as used */
        if ((idx_t)val >= used_cap)
            ensure_used_cap((idx_t)val + 1);
        used_flag[(idx_t)val] = 1;

        /* Ensure a_seq has room for index i */
        if (i > a_cap) {
            idx_t new_cap = a_cap ? a_cap * 2 : 2;
            if (new_cap < i)
                new_cap = i;
            seq_t *new_a = (seq_t *)realloc(a_seq, (new_cap + 1) * sizeof(seq_t));
            if (!new_a)
                exit(1);
            a_seq = new_a;
            a_cap = new_cap;
        }
        a_seq[i] = val;
    }
}

/* Public entry point: returns a(n). */
seq_t a(idx_t n)
{
    ensure_a_len(n);
    return a_seq[n];
}
