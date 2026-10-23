/*
 * A129802  Possible bases for Pepin's primality test for Fermat numbers.
 * https://oeis.org/A129802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1784, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A129802 – Possible bases for Pepin's test for Fermat numbers.
   The sequence consists of all positive integers of the form 2^k m
   (k ≥ 0, m odd, m > 1) such that the odd part m satisfies
   that Jacobi(2^(2^n)+1, m) == -1 for all n = r, r+1, … , r+ord(2,d)-1,
   where ord(2,m) = 2^r d.  The sequence is output in increasing order.
   Implementation uses a priority queue that stores the current smallest
   candidate.  Each key is doubled whenever it is extracted.  Only odd
   bases m are tested for validity once. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/jacobi_symbol.h"

/* ---------- modular arithmetic helpers ---------- */
static inline uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}
static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1ULL) res = modmul64(res, base, mod);
        base = modmul64(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}

/* ---------- prime sieve up to a fixed limit ---------- */
#define PRIME_SIEVE_LIMIT 4000000
static uint32_t primes[300000];
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void) {
    if (primes_initialized) return;
    bool *is_comp = (bool*)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_comp[i]) primes[prime_cnt++] = (uint32_t)i;
    free(is_comp);
    primes_initialized = true;
}

/* ----------- Euler φ for 64‑bit integers ----------- */
static uint64_t calc_phi(uint64_t n) {
    uint64_t res = n;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            res -= res / p;
        }
    }
    if (tmp > 1) res -= res / tmp;
    return res;
}

/* ------- multiplicative order  (a mod m, gcd(a,m)=1) ------ */
static uint64_t multiplicative_order(uint64_t a, uint64_t m) {
    if (m == 1) return 1;
    uint64_t phi = calc_phi(m);
    uint64_t ord = phi;
    uint64_t tmp = ord;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            while (ord % p == 0 && modpow64(a, ord / p, m) == 1)
                ord /= p;
        }
    }
    if (tmp > 1) {
        uint64_t p = tmp;
        while (ord % p == 0 && modpow64(a, ord / p, m) == 1)
            ord /= p;
    }
    return ord;
}

/* ---------- test whether odd m is a valid odd part ---------- */
static bool is_valid_odd_m(uint64_t m) {
    if (m <= 1 || (m & 1ULL) == 0) return false;
    uint64_t ord2 = multiplicative_order(2, m);          /* ord(2,m) */
    unsigned r = (unsigned)__builtin_ctzll(ord2);         /* exponent of 2 */
    uint64_t d = ord2 >> r;                              /* odd part */
    uint64_t ord2d = (d == 1) ? 1 : multiplicative_order(2, d);  /* ord(2,d) */
    /* compute 2^n mod ord2 efficiently */
    uint64_t expN = 1 % ord2;                             /* 2^0 mod ord2 */
    for (uint64_t i = 0; i < r; ++i)                    /* advance to 2^r */
        expN = (expN << 1) % ord2;
    for (uint64_t i = 0; i < ord2d; ++i) {              /* check n = r .. r+ord2d-1 */
        uint64_t val_mod = (modpow64(2ULL, expN, m) + 1ULL) % m;
        if (jacobi_symbol(val_mod, m) != -1) return false;
        expN = (expN << 1) % ord2;  /* next 2^n */
    }
    return true;
}

/* ---------- binary min‑heap for seq_t ---------- */
struct Heap {
    seq_t *data;
    size_t size;
    size_t capacity;
};
static void heap_init(struct Heap *h) {
    h->size = 0;
    h->capacity = 16;
    h->data = (seq_t*)malloc(h->capacity * sizeof(seq_t));
}
static void heap_push(struct Heap *h, seq_t val) {
    if (h->size == h->capacity) {
        h->capacity <<= 1;
        h->data = (seq_t*)realloc(h->data, h->capacity * sizeof(seq_t));
    }
    size_t i = h->size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p] <= val) break;
        h->data[i] = h->data[p];
        i = p;
    }
    h->data[i] = val;
}
static void heap_pop(struct Heap *h, seq_t *out) {
    *out = h->data[0];
    seq_t last = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= h->size) break;
        size_t r = l + 1;
        size_t child = (r < h->size && h->data[r] < h->data[l]) ? r : l;
        if (last <= h->data[child]) break;
        h->data[i] = h->data[child];
        i = child;
    }
    h->data[i] = last;
}
static seq_t heap_peek(const struct Heap *h) { return h->data[0]; }
static bool heap_empty(const struct Heap *h) { return h->size == 0; }

/* ---------- sequence function ---------- */
seq_t a(idx_t n) {
    static seq_t *values = NULL;
    static size_t values_len = 0;
    static size_t values_cap = 0;
    static struct Heap heap;
    static bool heap_initialized = false;
    static uint64_t next_odd = 3;          /* first odd > 1 */

    init_primes();
    if (!heap_initialized) {
        heap_init(&heap);
        heap_initialized = true;
    }

    if (n == 0) return 0; /* safety guard */

    while (values_len < (size_t)n) {
        /* generate valid odd m until the heap minimum is at least the
           next odd that we are inserting (ensures we keep the heap
           full enough to produce the smallest missing terms). */
        while (heap_empty(&heap) || next_odd <= heap_peek(&heap)) {
            uint64_t m = next_odd;
            next_odd += 2;
            if (is_valid_odd_m(m))
                heap_push(&heap, (seq_t)m);
        }

        /* pop smallest value and store it */
        seq_t v;
        heap_pop(&heap, &v);
        if (values_len == values_cap) {
            values_cap = values_cap ? values_cap << 1 : 16;
            values = (seq_t*)realloc(values, values_cap * sizeof(seq_t));
        }
        values[values_len++] = v;

        /* push the next power of two for this base if it will not overflow
           the maximum representable 64‑bit value (for 64‑bit builds). */
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            if (v <= ((seq_t)UINT64_MAX >> 1))
                heap_push(&heap, v * (seq_t)2);
        } else {
            heap_push(&heap, v * (seq_t)2);
        }
    }

    return values[n - 1];
}
