/*
 * A069823  Nonprime numbers k for which there is no x < k such that phi(x) =
 *          phi(k).
 * https://oeis.org/A069823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 5843, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, hash_phi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069823 – nonprime numbers k such that no smaller x has φ(x)=φ(k) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/calc_phi.h"
#include "../utils/is_prime.h"
#include "../utils/hash_phi.h"

/* Hash table for tracking already seen φ values */
static size_t table_size   = 0;        /* table capacity (power of two) */
static size_t table_count  = 0;        /* number of occupied slots */
static seq_t  *table_keys   = NULL;    /* keys (φ values) */
static bool   *table_flag   = NULL;    /* occupancy flags */

/* Allocate / resize hash table to given capacity (must be power of two) */
static void table_resize(size_t new_size) {
    seq_t *old_keys = table_keys;
    bool  *old_flag = table_flag;

    table_keys = (seq_t *)malloc(new_size * sizeof(seq_t));
    table_flag = (bool *)calloc(new_size, sizeof(bool));
    table_size = new_size;

    /* rehash old entries */
    for (size_t i = 0; i < new_size / 2; ++i) {
        if (old_flag[i]) {
            seq_t k = old_keys[i];
            size_t idx = hash_phi(k) & (table_size - 1);
            while (table_flag[idx]) idx = (idx + 1) & (table_size - 1);
            table_keys[idx] = k;
            table_flag[idx] = true;
        }
    }

    free(old_keys);
    free(old_flag);
}

/* Initialise hash table if not already */
static void table_init(void) {
    if (!table_keys) {
        table_part: /* first power of two >= 128 */
        table_size  = 128;
        table_keys  = (seq_t *)malloc(table_size * sizeof(seq_t));
        table_flag  = (bool *)calloc(table_size, sizeof(bool));
    }
}

/* Check whether key is already in table */
static bool table_contains(seq_t key) {
    if (!table_keys) return false;
    size_t idx = hash_phi(key) & (table_size - 1);
    while (table_flag[idx]) {
        if (table_keys[idx] == key) return true;
        idx = (idx + 1) & (table_size - 1);
    }
    return false;
}

/* Insert key into table (if not present) */
static void table_insert(seq_t key) {
    if (!table_keys) table_init();

    /* Resize if load factor exceeds 0.75 */
    if (table_count * 4 >= table_size * 3) {
        table_resize(table_size * 2);
    }

    size_t idx = hash_phi(key) & (table_size - 1);
    while (table_flag[idx]) {
        if (table_keys[idx] == key) return; /* already present */
        idx = (idx + 1) & (table_size - 1);
    }
    table_keys[idx] = key;
    table_flag[idx] = true;
    ++table_count;
}

/* Storage for sequence terms */
static seq_t *seq_terms   = NULL;
static size_t terms_cap   = 0;
static idx_t  terms_count = 0;
static idx_t  current_k   = 0;

/* Grow terms array */
static void grow_terms(void) {
    size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq_terms) {
        memcpy(new_arr, seq_terms, terms_count * sizeof(seq_t));
        free(seq_terms);
    }
    seq_terms = new_arr;
    terms_cap = new_cap;
}

/* Main routine: returns a(n) for n >= 1 */
seq_t a(idx_t n) {
    /* Ensure tables are initialized */
    if (!table_keys) table_init();
    if (!seq_terms) grow_terms();

    /* Generate terms until we have at least n */
    while (terms_count < n) {
        ++current_k;
        seq_t phi_k = calc_phi((seq_t)current_k);

        bool first = !table_contains(phi_k);
        table_insert(phi_k);

        if (first && !is_prime(current_k)) {
            if (terms_count == terms_cap) grow_terms();
            seq_terms[terms_count++] = current_k;
        }
    }

    return seq_terms[n - 1]; /* a(1) is the first stored term */
}
