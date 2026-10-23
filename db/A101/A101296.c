/*
 * A101296  n has the a(n)-th distinct prime signature.
 * https://oeis.org/A101296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = rank of the prime–signature of n. * The rank is the
 *            position (1‑based) of the least representative of * that
 *            signature when all least representatives are ordered by value
 * Range:     u8 n <= 2943, u16 n <= 9510, u32 n <= 9510, u64 n <= 9510,
 *            u128 n <= 9510, big any n
 * Helpers:   extract_signature
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/extract_signature.h"

/* --------- small prime list, enough for signature lengths we need -------- */
static const uint64_t small_primes[] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL
};
#define SMALL_PRIME_COUNT  (sizeof(small_primes)/sizeof(uint64_t))

/* --------- signature information ----------------------------------------- */
typedef struct {
    uint8_t  len;          /* number of exponents */
    uint8_t  exps[16];     /* exponents sorted ascending */
    uint16_t rank;         /* 1‑based rank of this signature */
} sig_entry_t;

static sig_entry_t *sig_table  = NULL;
static size_t      sig_count   = 0;
static size_t      sig_cap     = 0;

/* --------- least representative ranks ----------------------------------- */
static uint16_t *least_rank   = NULL;   /* least_rank[m] = rank of least number m (0 if none) */
static size_t   least_cap     = 0;
static idx_t    global_max    = 0;      /* highest index processed so far */

/* --------- array grow helpers ------------------------------------------- */
static void grow_sig_table(void) {
    size_t new_cap = (sig_cap == 0) ? 16 : sig_cap * 2;
    sig_entry_t *new_tab = (sig_entry_t*)realloc(sig_table, new_cap * sizeof(sig_entry_t));
    if (!new_tab) abort();
    sig_table = new_tab;
    sig_cap   = new_cap;
}

static void grow_least_rank(size_t need) {
    if (need <= least_cap) return;
    size_t new_cap = (least_cap == 0) ? 64 : least_cap;
    while (new_cap <= need) new_cap <<= 1;
    uint16_t *new_arr = (uint16_t*)realloc(least_rank, new_cap * sizeof(uint16_t));
    if (!new_arr) abort();
    /* zero‑initialize new part */
    memset(new_arr + least_cap, 0, (new_cap - least_cap) * sizeof(uint16_t));
    least_rank = new_arr;
    least_cap   = new_cap;
}

/* --------- least number from a signature ------------------------------ */
static seq_t least_from_signature(const uint8_t *exps, uint8_t len) {
    seq_t prod = 1;
    for (uint8_t i = 0; i < len; ++i) {
        seq_t p = small_primes[len-1-i];
        for (uint8_t e = 0; e < exps[i]; ++e) prod *= p;
    }
    return prod;
}

/* --------- generate data up to a given index --------------------------- */
static void generate_up_to(idx_t limit) {
    if (limit <= global_max) return;
    for (idx_t i = global_max + 1; i <= limit; ++i) {
        uint8_t exps[16], len;
        extract_signature(i, exps, &len);
        /* linear search for existing signature */
        size_t pos = sig_count;
        for (size_t j = 0; j < sig_count; ++j) {
            if (sig_table[j].len != len) continue;
            bool same = true;
            for (uint8_t k = 0; k < len; ++k)
                if (sig_table[j].exps[k] != exps[k]) { same = false; break; }
            if (same) { pos = j; break; }
        }
        if (pos == sig_count) {            /* new signature */
            if (sig_count >= sig_cap) grow_sig_table();
            sig_table[sig_count].len  = len;
            memcpy(sig_table[sig_count].exps, exps, len);
            sig_table[sig_count].rank = (uint16_t)(sig_count + 1);
            pos = sig_count;
            ++sig_count;
            /* record rank of this least number */
            grow_least_rank((size_t)i + 1);
            least_rank[i] = sig_table[pos].rank;
        }
    }
    global_max = limit;
}

/* --------- main function ------------------------------------------------ */
seq_t a(idx_t n) {
    if (n < 1) abort();
    uint8_t exps[16], len;
    extract_signature(n, exps, &len);
    seq_t least = least_from_signature(exps, len);
    /* ensure all numbers up to 'least' have been processed */
    generate_up_to((idx_t)least);
    return (seq_t)least_rank[(size_t)least];
}
