/*
 * A076063  Group the natural numbers so that the n-th group contains n
 *          numbers whose sum is a multiple of the n-th composite number.
 * https://oeis.org/A076063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 1540, u32 n <= 1540, u64 n <= 1540,
 *            u128 n <= 1540, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A076063.
   a(n) is the last element in the n‑th group of a greedy grouping
   of the natural numbers where each group of size n has a sum
   divisible by the n‑th composite number. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Cache of already generated terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Number of rows that have been processed */
static idx_t rows_done   = 0;

/* Smallest natural number that is not yet used */
static idx_t smallest_unused = 1;

/* Bitset of used numbers */
static uint8_t *used_bits  = NULL;
static size_t used_bytes   = 0;

/* Composite numbers list */
static seq_t *composites   = NULL;
static size_t composites_len = 0;
static size_t composites_cap = 0;

/* Sieve of primes up to LIMIT for producing composites */
static bool *isprime_arr   = NULL;
static size_t isprime_size = 0;

/* --------------------------------------------------------------------- */
/* Bitset helpers                                                     */
static inline bool is_used(idx_t x)
{
    size_t byte = (size_t)(x >> 3);
    if (byte >= used_bytes) return false;
    return (used_bits[byte] >> (x & 7)) & 1;
}

static void ensure_used_capacity(idx_t x)
{
    size_t byte = (size_t)(x >> 3);
    if (byte < used_bytes) return;
    size_t new_len = used_bytes ? used_bytes << 1 : 128;
    while (new_len <= byte) new_len <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used_bits, new_len);
    if (!tmp) exit(1);
    memset(tmp + used_bytes, 0, new_len - used_bytes);
    used_bits  = tmp;
    used_bytes = new_len;
}

static inline void mark_used(idx_t x)
{
    ensure_used_capacity(x);
    size_t byte = (size_t)(x >> 3);
    used_bits[byte] |= 1 << (x & 7);
}

/* --------------------------------------------------------------------- */
/* Composite numbers initialisation   */
static void init_composites(void)
{
    if (composites) return;                   /* already done */

    const size_t LIMIT = 200000;              /* enough for n≈10000   */
    isprime_size = LIMIT + 1;
    isprime_arr = (bool *)calloc(isprime_size, 1);    /* all zero, will mark primes */
    if (!isprime_arr) exit(1);
    for (size_t i = 2; i < isprime_size; ++i) isprime_arr[i] = 1;

    for (size_t p = 2; p * p < isprime_size; ++p) {
        if (isprime_arr[p]) {
            for (size_t j = p * p; j < isprime_size; j += p)
                isprime_arr[j] = 0;
        }
    }

    composites_cap = 128;                     /* will grow as needed   */
    composites = (seq_t *)calloc(composites_cap, sizeof(seq_t));
    if (!composites) exit(1);
    for (size_t i = 4; i < isprime_size; ++i) {
        if (!isprime_arr[i]) {
            if (composites_len == composites_cap) {
                composites_cap <<= 1;
                composites = (seq_t *)realloc(composites,
                                              composites_cap * sizeof(seq_t));
                if (!composites) exit(1);
            }
            composites[composites_len++] = (seq_t)i;
        }
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure composites are ready */
    init_composites();

    /* Expand cache as necessary */
    while (cache_len < (size_t)n) {
        idx_t r = rows_done + 1;               /* next row number   */
        seq_t comp = composites[r-1];          /* r‑th composite   */

        seq_t sum_mod = 0;                     /* modulo of partial sum */

        /* first r‑1 smallest unused numbers */
        for (idx_t i = 0; i < r-1; ++i) {
            while (is_used(smallest_unused)) smallest_unused++;
            seq_t val = (seq_t)smallest_unused;
            mark_used(smallest_unused);

            if (cache_len == cache_cap) {      /* grow cache   */
                size_t new_cap = cache_cap ? cache_cap << 1 : 1024;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
            sum_mod = (sum_mod + val) % comp;
            smallest_unused++;                /* next candidate   */
        }

        /* last number: smallest unused satisfying the congruence */
        seq_t target = (comp - sum_mod) % comp;   /* 0 … comp‑1      */
        idx_t cand = smallest_unused;            /* start from smallest unused */
        seq_t rem  = (seq_t)cand % comp;
        seq_t diff = (target >= rem) ? (target - rem) : (target + comp - rem);
        cand += (idx_t)diff;                    /* first candidate with correct residue */

        while (is_used(cand)) {                 /* skip already used numbers   */
            cand += (idx_t)comp;
        }

        mark_used(cand);
        if (cache_len == cache_cap) {           /* grow cache   */
            size_t new_cap = cache_cap ? cache_cap << 1 : 1024;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)cand;
        /* smallest_unused remains pointing to the next unused number. */

        rows_done++;
    }

    return cache[n-1];
}
