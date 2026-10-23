/*
 * A175751  Numbers with 44 divisors.
 * https://oeis.org/A175751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* OEIS A175751: Numbers with 44 divisors.
   Numbers of the forms p^43, p^21*q, p^10*q^3 and p^10*q*r with distinct primes. */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/build_primes_upto.h"

/* Dynamic array for the sequence terms. */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Append a value to the dynamic array. */
static inline void push_val(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 128;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = v;
}

/* Comparator for qsort on seq_t. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* One‑time initialization: generate all numbers with exactly 44 divisors
   up to a fixed upper bound. */
static bool initialized = false;

/* Our upper bound: big enough to contain at least the first 10 000 terms. */
#define LIMIT ( (seq_t)1000000000ULL )   /* 1 000 000 000 */

static void init_sequence(void)
{
    if (initialized) return;

    /* Limit for primes we need to enumerate – sufficient for all patterns
       below LIMIT as explained in the comments. */
    const idx_t PRIME_LIMIT = 1000000;   /* 1 000 000 – about 78 498 primes */

    /* Build array of primes up to PRIME_LIMIT. */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(PRIME_LIMIT, &primes, &primes_cnt);

    /* 1. p^43 (only prime 2 produces a value <= LIMIT). */
    seq_t p43 = big_pow((seq_t)2, (seq_t)43);
    if (p43 <= LIMIT) push_val(p43);

    /* 2. p^21 * q  (p and q distinct). */
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p21 = big_pow((seq_t)primes[i], (seq_t)21);
        if (p21 > LIMIT) break;           /* larger p gives larger p21 */
        for (size_t j = 0; j < primes_cnt; ++j) {
            if (primes[j] == primes[i]) continue;
            seq_t val = seq_mul(p21, (seq_t)primes[j]);
            if (val > LIMIT) break;        /* q increases ⇒ val increases */
            push_val(val);
        }
    }

    /* 3. p^10 * q^3  (p and q distinct). */
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p10 = big_pow((seq_t)primes[i], (seq_t)10);
        if (p10 > LIMIT) break;           /* larger p gives larger p10 */
        for (size_t j = 0; j < primes_cnt; ++j) {
            if (primes[j] == primes[i]) continue;
            seq_t q3 = big_pow((seq_t)primes[j], (seq_t)3);
            seq_t val = seq_mul(p10, q3);
            if (val > LIMIT) break;        /* q increases ⇒ val increases */
            push_val(val);
        }
    }

    /* 4. p^10 * q * r  (p,q,r distinct, q < r to avoid duplicates). */
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p10 = big_pow((seq_t)primes[i], (seq_t)10);
        if (p10 > LIMIT) break;
        for (size_t j = 0; j < primes_cnt; ++j) {
            if (primes[j] == primes[i]) continue;
            seq_t pq = seq_mul(p10, (seq_t)primes[j]);
            if (pq > LIMIT) continue;       /* may still be usable with smaller r */
            for (size_t k = j + 1; k < primes_cnt; ++k) {
                if (primes[k] == primes[i]) continue;
                seq_t val = seq_mul(pq, (seq_t)primes[k]);
                if (val > LIMIT) break;      /* r increases ⇒ val increases */
                push_val(val);
            }
        }
    }

    /* Sort and remove duplicates. */
    qsort(seq_arr, seq_len, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < seq_len; ++i) {
        if (uniq == 0 || seq_arr[i] != seq_arr[uniq - 1]) {
            seq_arr[uniq++] = seq_arr[i];
        }
    }
    seq_len = uniq;

    free(primes);
    initialized = true;
}

/* The required public function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */
    init_sequence();
    if ((size_t)n > seq_len) return 0;   /* out of pre‑computed range */
    return seq_arr[(size_t)n - 1];
}
