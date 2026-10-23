/*
 * A162332  Numbers k such that k^2 is an anagram of a triangular number.
 * https://oeis.org/A162332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: a(n) = the n-th integer k such that k^2 is an anagram of a triangular number.
 * a(1) = 1, a(2) = 6, a(3) = 19, … (OEIS A162332)
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/digit_len.h"

/* ------------------------------------------------------------------------
 *  Hash table for keys representing the digit multiset of a number.
 *  Keys are 64‑bit integers generated from the counts of each digit.
 * ------------------------------------------------------------------------ */
#define HASH_SIZE (1 << 20)          /* 1,048,576 entries – plenty for the needed sizes */
#define HASH_MASK (HASH_SIZE - 1)

static uint64_t *hash_set = NULL;

/* (Re)initialize the hash table */
static void hash_set_init(void)
{
    hash_set = (uint64_t *)calloc(HASH_SIZE, sizeof(uint64_t));
}

/* Simple linear‑probe insert – duplicates are ignored */
static inline void hash_set_insert(uint64_t key)
{
    size_t h = (key ^ (key >> 32)) & HASH_MASK;
    while (hash_set[h] != 0) {
        if (hash_set[h] == key) {
            return;               /* already present */
        }
        h = (h + 1) & HASH_MASK;
    }
    hash_set[h] = key;
}

/* Test for membership */
static inline bool hash_set_contains(uint64_t key)
{
    size_t h = (key ^ (key >> 32)) & HASH_MASK;
    while (hash_set[h] != 0) {
        if (hash_set[h] == key) {
            return true;
        }
        h = (h + 1) & HASH_MASK;
    }
    return false;
}

/* ------------------------------------------------------------------------
 *  Create a unique key from the digit multiplicities.
 *  key = Σ cnt[d] * 11^d   (gives a bijection for the small counts we use)
 * ------------------------------------------------------------------------ */
static const uint64_t pow11_arr[10] = {
    10000000000ULL, 1000000000ULL, 100000000ULL, 10000000ULL,
    1000000ULL, 100000ULL, 10000ULL, 1000ULL, 100ULL, 10ULL
};

static uint64_t digit_key(const seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    uint64_t key = 0;
    for (int d = 0; d < 10; ++d) {
        key += (uint64_t)cnt[d] * pow11_arr[d];
    }
    return key;
}

/* ------------------------------------------------------------------------
 *  Keep a running list of triangular numbers.
 *  tri   = T(step)   (current triangular)
 *  step  = m of T(m)
 *  tri_len = digit_len(tri)
 *  max_processed_len = largest length of triangulars that have been hashed
 * ------------------------------------------------------------------------ */
static seq_t tri   = 1;         /* T(1)  */
static seq_t step  = 1;         /* m for T(m) */
static int   tri_len = 1;
static int   max_processed_len = 0;   /* 0 ensures initial insertion of T(1) */

/* Ensure that all triangular numbers with digit length <= limit_len
 * have been inserted into the hash table. */
static void ensure_triangles_up_to(int limit_len)
{
    if (limit_len <= max_processed_len) {
        return;   /* already processed */
    }

    while (tri_len <= limit_len) {
        hash_set_insert(digit_key(tri));
        ++step;
        tri = tri + step;          /* next triangular number */
        tri_len = digit_len(tri);
    }
    max_processed_len = limit_len;
}

/* ------------------------------------------------------------------------
 *  Main sequence function.
 * ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ---------- static state ---------- */
    static bool initialized = false;
    static seq_t *seq = NULL;     /* dynamic array of found k's */
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static seq_t next_k = 1;      /* next candidate k to test */

    if (!initialized) {
        hash_set_init();
        initialized = true;
    }

    /* Build sequence until we have n terms */
    while (seq_len < (size_t)n) {
        seq_t k = next_k++;
        seq_t ksq = k * k;

        int len = digit_len(ksq);
        ensure_triangles_up_to(len);

        if (hash_set_contains(digit_key(ksq))) {
            /* grow the table if needed */
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = k;
        }
    }

    return seq[n - 1];
}
