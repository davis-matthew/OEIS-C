/*
 * A076524  For each solution x>0 to 3*prime(n)+2*x = prime(n+2+x), the
 *          sequence lists prime(n) once.
 * https://oeis.org/A076524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   exit_if_null, hash_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A076524
 * For each solution x>0 to 3*prime(n)+2*x = prime(n+2+x),
 * the sequence lists prime(n) once.
 * The terms are ordered by increasing k=n+2+x; each matching pair
 * (n,x) produces an independent term.
 *
 * Implementation: iterate primes, maintain a hash table of pending
 * prime indices n keyed by C_n = 3*prime(n)-2*n-4.
 * For each new prime at index k compute g_k = prime(k)-2*k;
 * all pending n with key==g_k and k>n+2 produce output prime(n).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/exit_if_null.h"
#include "../utils/hash_seq_t.h"

/* seq_t and idx_t are supplied by the build */
static const size_t BUCKET_SIZE = 131071; /* prime number > 1e5 */

/*** Prime table ***/
static uint64_t *prime_arr  = NULL; /* 1‑based: prime_arr[i] = p_i */
static size_t     prime_cap = 0;
static size_t     prime_len = 0;

/*** Hash table for pending n ***/
typedef struct Entry {
    seq_t   key;            /* C_n value */
    idx_t * nlist;          /* list of n values that produce this key */
    size_t  ncap;
    size_t  nlen;
    size_t  next;           /* next entry in bucket chain, or -1 */
} Entry;

static Entry *entries   = NULL;   /* array of entries */
static size_t  entries_cap = 0;
static size_t  entries_len = 0;
static size_t *bucket     = NULL; /* bucket heads, index into entries (or -1) */

/*** Output sequence ***/
static seq_t *seq_res = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static void init_globals(void);

/*** Prime generation ***/
static void ensure_prime_cap(size_t needed) {
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *new_pg = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    exit_if_null(new_pg);
    prime_arr = new_pg;
    prime_cap = new_cap;
}

static bool is_candidate_prime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 1; i <= prime_len; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

static void generate_next_prime(void) {
    uint64_t cand = (prime_len == 0) ? 2ULL : prime_arr[prime_len] + 2ULL;
    for (;;) {
        if (is_candidate_prime(cand)) {
            ++prime_len;
            ensure_prime_cap(prime_len + 1);
            prime_arr[prime_len] = cand;
            return;
        }
        cand += 2ULL;
    }
}

/*** Hash table operations ***/
static void ensure_entries_cap(size_t needed) {
    if (entries_cap >= needed) return;
    size_t new_cap = entries_cap ? entries_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    Entry *new_e = (Entry *)realloc(entries, new_cap * sizeof(Entry));
    exit_if_null(new_e);
    entries = new_e;
    entries_cap = new_cap;
}

static void add_pending(seq_t key, idx_t n) {
    size_t h = hash_seq_t(key) % BUCKET_SIZE;
    for (size_t idx = bucket[h]; idx != (size_t)-1; idx = entries[idx].next) {
        if (entries[idx].key == key) {
            Entry *e = &entries[idx];
            if (e->nlen == e->ncap) {
                size_t new_cap = e->ncap * 2;
                idx_t *new_list = (idx_t *)realloc(e->nlist, new_cap * sizeof(idx_t));
                exit_if_null(new_list);
                e->nlist = new_list;
                e->ncap = new_cap;
            }
            e->nlist[e->nlen++] = n;
            return;
        }
    }
    /* not found – create new entry */
    ensure_entries_cap(entries_len + 1);
    size_t idx = entries_len++;
    Entry *e = &entries[idx];
    e->key = key;
    e->ncap = 2;
    e->nlen = 1;
    e->nlist = (idx_t *)malloc(e->ncap * sizeof(idx_t));
    exit_if_null(e->nlist);
    e->nlist[0] = n;
    e->next = bucket[h];
    bucket[h] = idx;
}

static void ensure_seq_cap(size_t needed) {
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(seq_res, new_cap * sizeof(seq_t));
    exit_if_null(new_arr);
    seq_res = new_arr;
    seq_cap = new_cap;
}

static void append_output(seq_t val) {
    ensure_seq_cap(seq_len + 1);
    seq_res[seq_len++] = val;
}

/*** Process a newly generated prime ***/
static void process_prime(size_t k) {
    seq_t g = (seq_t)prime_arr[k] - (seq_t)2 * (seq_t)k;
    size_t h = hash_seq_t(g) % BUCKET_SIZE;
    for (size_t idx = bucket[h]; idx != (size_t)-1; idx = entries[idx].next) {
        if (entries[idx].key == g) {
            Entry *e = &entries[idx];
            for (size_t j = 0; j < e->nlen; ++j) {
                idx_t n = e->nlist[j];
                if (k > n + 2) {
                    seq_t val = (seq_t)prime_arr[n];
                    append_output(val);
                }
            }
        }
    }
}

/*** Initialization ***/
static void init_globals(void) {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* primes array */
    prime_cap = 16;
    prime_arr = (uint64_t *)calloc(prime_cap, sizeof(uint64_t));
    exit_if_null(prime_arr);
    prime_arr[1] = 2ULL;
    prime_arr[2] = 3ULL;
    prime_len = 2;

    /* hash table */
    bucket = (size_t *)calloc(BUCKET_SIZE, sizeof(size_t));
    exit_if_null(bucket);
    for (size_t i = 0; i < BUCKET_SIZE; ++i) bucket[i] = (size_t)-1;
    entries_cap = 16;
    entries_len = 0;
    entries = (Entry *)calloc(entries_cap, sizeof(Entry));
    exit_if_null(entries);

    /* output sequence */
    seq_cap = 16;
    seq_res = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    exit_if_null(seq_res);
    seq_len = 0;
}

static idx_t next_n_to_process = 2; /* next prime index to add as pending */

/*** Public API ***/
seq_t a(idx_t n) {
    init_globals();

    while (seq_len < (size_t)n) {
        /* Ensure current pending n has its prime available */
        if (prime_len < (size_t)next_n_to_process) {
            generate_next_prime();
        }

        /* Insert pending n with its key */
        seq_t key = (seq_t)3 * (seq_t)prime_arr[next_n_to_process]
                    - (seq_t)2 * (seq_t)next_n_to_process
                    - (seq_t)4;
        add_pending(key, next_n_to_process);
        ++next_n_to_process;

        /* Generate next prime and process any matches */
        generate_next_prime();          /* increases prime_len */
        process_prime(prime_len);
    }

    return seq_res[n - 1];
}
