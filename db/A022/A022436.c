/*
 * A022436  a(n) = c(n-1) + c(n-3) where c is the sequence of numbers not in
 *          a.
 * https://oeis.org/A022436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n-1) + c(n-3) where c is the sequence of positive
 *            integers not appearing in a. Initial values: a(0) = 1, a(1) = 4,
 *            a(2) = 6
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t *a_vals   = NULL;  /* computed a(0..) */
static idx_t  a_cap   = 0;
static idx_t  a_size  = 0;       /* number of a terms known */

static seq_t *c_vals   = NULL;  /* complement sequence c */
static idx_t  c_cap   = 0;
static idx_t  c_size  = 0;

static unsigned char *present = NULL;   /* bitmap of numbers already in a */
static idx_t  present_size    = 0;      /* length of present array */

static idx_t  next_candidate = 1;       /* next integer to test for c */
static bool   initialized      = false;

/* enlarge present array so that indices up to "needed" are legal */
static void grow_present_to(idx_t needed)
{
    idx_t new_size = present_size ? present_size : 8;
    while (new_size <= needed) new_size <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(present,
                                                new_size * sizeof(unsigned char));
    if (!tmp) abort();
    if (!present) {
        memset(tmp, 0, (size_t)new_size);
    } else {
        memset(tmp + present_size, 0, (size_t)(new_size - present_size));
    }
    present     = tmp;
    present_size = new_size;
}

/* enlarge any dynamic array of seq_t */
static void grow_arr(void **arr, idx_t *cap, idx_t needed)
{
    idx_t new_cap = *cap ? *cap : 8;
    while (new_cap <= needed) new_cap <<= 1;
    void *tmp = realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

/* initialisation of static state */
static void init(void)
{
    grow_arr((void**)&a_vals, &a_cap, 8);
    grow_arr((void**)&c_vals, &c_cap, 8);
    grow_present_to(64);  /* space for the first few markers */

    /* base values */
    a_vals[0] = (seq_t)1;
    a_vals[1] = (seq_t)4;
    a_vals[2] = (seq_t)6;
    a_size = 3;

    /* mark these values as present */
    size_t v1 = 1, v2 = 4, v3 = 6;
    if (v1 >= (size_t)present_size) grow_present_to(v1);
    if (v2 >= (size_t)present_size) grow_present_to(v2);
    if (v3 >= (size_t)present_size) grow_present_to(v3);
    present[v1] = 1;
    present[v2] = 1;
    present[v3] = 1;

    initialized = true;
}

/* main routine requested by the builds */
seq_t a(idx_t n)
{
    if (!initialized) init();

    /* already computed ? */
    if (n < a_size) return a_vals[n];

    /* extend the tables up to n */
    for (idx_t k = a_size; k <= n; ++k) {
        /* ensure the complement list has c(k-1) */
        while (c_size <= k - 1) {
            /* grow present array if needed */
            while (next_candidate >= present_size) grow_present_to(next_candidate);
            size_t cand = (size_t)next_candidate;
            if (!present[cand]) {
                if (c_size >= c_cap) grow_arr((void**)&c_vals, &c_cap, c_size);
                c_vals[c_size++] = (seq_t)cand;
            }
            ++next_candidate;
        }

        /* compute a(k) = c(k-1) + c(k-3) */
        seq_t val = c_vals[k - 1] + c_vals[k - 3];

        if (a_size >= a_cap) grow_arr((void**)&a_vals, &a_cap, a_size);
        a_vals[a_size++] = val;

        /* mark new value as present */
        size_t vi = (size_t)val;
        if (vi >= present_size) grow_present_to(vi);
        present[vi] = 1;
    }

    return a_vals[n];
}
