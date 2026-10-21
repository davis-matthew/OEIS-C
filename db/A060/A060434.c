/*
 * A060434  An inverse to Mertens's function: smallest k >= 2 such that
 *          Mertens's function |M(k)| (see A002321) is equal to n.
 * https://oeis.org/A060434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of Mertens’s function:
   a(n) = smallest k ≥ 2 such that |M(k)| = n
   where M(k) = μ(1) + μ(2) + … + μ(k) (Mertens' function).
   Offset 0 – a(0) = 2. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---------- bit‑set helpers ------------------------------------ */
static inline bool get_bit(const uint8_t *b, idx_t i) {
    return (b[(size_t)(i >> 3)] >> (i & 7)) & 1u;
}
static inline void set_bit(uint8_t *b, idx_t i) {
    b[(size_t)(i >> 3)] |= 1u << (i & 7);
}
/* --------------------------------------------------------------- */

/* ---------- globals -------------------------------------------- */
static bool initialized = false;

static idx_t limit = 0;                     /* largest k processed so far */
static int8_t  *mu = NULL;                  /* Möbius values (signed 8‑bit)   */
static uint8_t *composite = NULL;           /* bitset: composite numbers     */
static size_t mu_cap = 0, comp_cap = 0;     /* allocated array sizes          */

static uint32_t *primes = NULL;             /* primes list                    */
static size_t primes_cap = 0, primes_cnt = 0;

static idx_t *mapping = NULL;               /* mapping from |M(k)| to smallest k (idx_t) */
static size_t mapping_cap = 0;

static long long current_M = 0;              /* running sum of μ(1)+…+μ(k)   */

/* --------------------------------------------------------------- */

/* ---------- memory helpers ------------------------------------- */
static void grow_mu(size_t new_cap) {
    int8_t *tmp = (int8_t *)realloc(mu, new_cap * sizeof(int8_t));
    if (!tmp) exit(EXIT_FAILURE);
    memset(tmp + mu_cap, 0, (new_cap - mu_cap) * sizeof(int8_t));
    mu = tmp; mu_cap = new_cap;
}

static void grow_comp(size_t new_cap) {
    uint8_t *tmp = (uint8_t *)realloc(composite, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    memset(tmp + comp_cap, 0, new_cap - comp_cap);
    composite = tmp; comp_cap = new_cap;
}

static void grow_primes(size_t new_cap) {
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp; primes_cap = new_cap;
}

static void grow_mapping(size_t new_cap) {
    idx_t *tmp = (idx_t *)realloc(mapping, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    memset(tmp + mapping_cap, 0, (new_cap - mapping_cap) * sizeof(idx_t));
    mapping = tmp; mapping_cap = new_cap;
}

/* --------------------------------------------------------------- */

/* ---------- extend computation up to new_limit (inclusive) ----- */
static void extend_to(idx_t new_limit) {
    if (new_limit <= limit) return;

    /* --- allocate / enlarge auxiliary arrays -------------------- */
    if (new_limit + 1 > mu_cap)        grow_mu(new_limit + 1);
    size_t comp_needed = (size_t)((new_limit >> 3) + 1);
    if (comp_needed > comp_cap)        grow_comp(comp_needed);

    /* estimate prime count needed for the sieve */
    if (new_limit > 1) {
        size_t approx = (size_t)(new_limit / (log((double)new_limit) + 1.0)) + 10;
        if (approx > primes_cap)        grow_primes(approx);
    }

    /* --- linear sieve and Mertens computation ------------------- */
    for (idx_t i = limit + 1; i <= new_limit; ++i) {
        /* process Möbius value for i */
        if (i == 1) {
            mu[1] = 1;                /* μ(1) = 1 */
        } else {
            if (!get_bit(composite, i)) {          /* i is prime */
                if (primes_cnt == primes_cap) grow_primes(primes_cap * 2);
                primes[primes_cnt++] = (uint32_t)i;
                mu[i] = -1;
            }
            /* propagate to multiples of already known primes */
            for (size_t pi = 0; pi < primes_cnt; ++pi) {
                uint32_t p = primes[pi];
                idx_t prod = p * i;
                if (prod > new_limit) break;
                set_bit(composite, prod);
                if (i % p == 0) {                    /* square factor */
                    mu[prod] = 0;
                    break;
                } else {
                    mu[prod] = -mu[i];
                }
            }
        }

        /* update the running Mertens sum */
        current_M += (long long)mu[i];
        idx_t absM = (current_M >= 0) ? (idx_t)current_M
                                      : (idx_t)(-current_M);

        /* record the first occurrence of this |M(k)| */
        if (i >= 2) {
            if (absM >= mapping_cap) {
                size_t newcap = mapping_cap ? mapping_cap * 2 : 1;
                while (absM >= newcap) newcap <<= 1;
                grow_mapping(newcap);
            }
            if (mapping[absM] == 0) {
                mapping[absM] = i;
            }
        }
    }
    limit = new_limit;
}

/* --------------------------------------------------------------- */

/* ---------- pre‑computation for requested n ------------------- */
static void precompute_to(idx_t n) {
    if (!initialized) {
        /* minimal initial allocation */
        mu = NULL;             mu_cap = 0;
        composite = NULL;     comp_cap = 0;
        primes = NULL;         primes_cap = 0; primes_cnt = 0;
        mapping = NULL;        mapping_cap = 0;

        grow_mapping(1);        /* allocate mapping[0] */
        initialized = true;
    }

    /* extend computation until mapping[n] is known */
    const idx_t INCR = 1 << 20;   /* 1 048 576, balances memory & speed */
    while (n >= mapping_cap || mapping[n] == 0) {
        extend_to(limit + INCR);
    }
}

/* --------------------------------------------------------------- */

/* ---------- public function ------------------------------------ */
seq_t a(idx_t n) {
    precompute_to(n);
    /* mapping[n] holds the smallest k, cast to seq_t for return */
    return (seq_t) mapping[n];
}
