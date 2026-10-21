/*
 * A088177  a(1)=1, a(2)=1; for n>2, a(n) is the smallest positive integer
 *          such that the products a(i)*a(i+1), i=1..n-1, are all distinct.
 * https://oeis.org/A088177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 599, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hash_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest positive integer such that the adjacent
   products a(i)∙a(i+1) for i = 1..n−1 are all distinct.
   The sequence starts a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=3, … */

/* The implementation stores already computed terms in a dynamic array
   and maintains a hash table of all produced products for O(1)
   duplicate lookup. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/hash_value.h"

/* ------------------------------------------------------------------ */
/* Dynamic array for the sequence ------------------------------------ */
static seq_t *seq_arr  = NULL;  /* a(1)…a(k) stored 0‑based */
static size_t seq_cap  = 0;     /* array capacity (count of elements) */
static idx_t   seq_len  = 0;    /* number of elements currently stored */

/* Ensure seq_arr can hold at least 'need' elements.  */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 8;
    while (newcap < need) newcap <<= 1;
    seq_t *newptr = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
    if (!newptr) abort();
    seq_arr = newptr;
    seq_cap = newcap;
}

/* ------------------------------------------------------------------ */
/* Simple open‑addressing hash table for the products ---------------- */
static seq_t *prod_hash = NULL; /* table of products, 0 == empty */
static size_t hash_mask  = 0;   /* size of table - 1, power of two */
static size_t hash_cnt   = 0;   /* number of stored values */

/* Initialise the hash table with a small power‑of‑two capacity. */
static void prod_hash_init(void)
{
    if (prod_hash) return;
    hash_mask = 15; /* table size 16 */
    prod_hash = (seq_t *)calloc(hash_mask + 1, sizeof(seq_t));
    if (!prod_hash) abort();
    hash_cnt = 0;
}

/* Insert 'value' into the hash table.  Handles table expansion.  */
static void prod_hash_insert(seq_t value)
{
    prod_hash_init();

    /* Load factor check: expand if more than 70% full. */
    if ((hash_cnt + 1) * 10 > (hash_mask + 1) * 7) {
        size_t old_cap  = hash_mask + 1;
        size_t new_cap  = old_cap << 1;
        size_t new_mask = new_cap - 1;

        seq_t *new_tab = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_tab) abort();

        /* Rehash existing entries. */
        for (size_t i = 0; i < old_cap; ++i) {
            seq_t v = prod_hash[i];
            if (!v) continue;
            size_t pos = hash_value(v) & new_mask;
            while (new_tab[pos]) pos = (pos + 1) & new_mask;
            new_tab[pos] = v;
        }

        free(prod_hash);
        prod_hash = new_tab;
        hash_mask  = new_mask;
    }

    /* Probe for insertion slot. */
    size_t pos = hash_value(value) & hash_mask;
    while (prod_hash[pos]) pos = (pos + 1) & hash_mask;
    prod_hash[pos] = value;
    ++hash_cnt;
}

/* Return true if 'value' is already stored in the table. */
static bool prod_hash_contains(seq_t value)
{
    if (!prod_hash) return false;    /* table empty */

    size_t pos = hash_value(value) & hash_mask;
    while (prod_hash[pos]) {
        if (prod_hash[pos] == value) return true;
        pos = (pos + 1) & hash_mask;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety guard; sequence starts at 1 */

    /* Initialise state on first call. */
    if (!seq_arr) {
        ensure_seq_capacity(8);
        seq_arr[0] = (seq_t)1;  /* a(1) */
        seq_arr[1] = (seq_t)1;  /* a(2) */
        seq_len = 2;

        prod_hash_init();
        prod_hash_insert((seq_t)1 * (seq_t)1);  /* product a(1)∙a(2) */
    }

    /* Extend sequence up to required index n. */
    for (idx_t idx = seq_len + 1; idx <= n; ++idx) {
        seq_t prev = seq_arr[idx - 2];  /* a(idx-1) */
        seq_t cur  = (seq_t)1;          /* candidate for a(idx) */

        /* Search the smallest cur such that prev*cur is new. */
        seq_t product;
        while (true) {
            product = prev * cur;
            if (!prod_hash_contains(product)) break;
            ++cur;
        }

        /* Store new term and its product. */
        ensure_seq_capacity(seq_len + 1);
        seq_arr[seq_len] = cur;
        ++seq_len;

        prod_hash_insert(product);
    }

    return seq_arr[n - 1];
}
