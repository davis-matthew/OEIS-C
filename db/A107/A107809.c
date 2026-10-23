/*
 * A107809  a(1) = prime(9), for n >= 2, a(n) is the smallest prime not
 *          previously used which contains a digit from a(n-1).
 * https://oeis.org/A107809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107809: a(1)=prime(9). For n≥2, a(n) is the smallest prime
   not yet used that contains a digit from a(n‑1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Miller‑Rabin primality test for 64‑bit integers ---------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m) {
    return (unsigned __int128)a * b % m;
}

static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, m);
        a = mul_mod(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool is_prime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- storage of primes and used flags ---------- */
struct PrimeList {
    uint64_t *p;   /* primes */
    bool     *used;/* whether prime has been used in sequence */
    size_t    cap; /* array capacity */
    size_t    len; /* number of stored primes */
    uint64_t  last;/* last number checked for primality */
} primes = {NULL, NULL, 0, 0, 1};

static void ensure_prime_list(void) {
    if (!primes.p) {
        primes.cap = 64;
        primes.p   = (uint64_t*)malloc(primes.cap * sizeof(uint64_t));
        primes.used= (bool*)calloc(primes.cap, sizeof(bool));
        primes.len = 0;
    }
}

static void add_prime(uint64_t v) {
    if (primes.len == primes.cap) {
        primes.cap <<= 1;
        primes.p    = (uint64_t*)realloc(primes.p, primes.cap * sizeof(uint64_t));
        primes.used = (bool*)realloc(primes.used, primes.cap * sizeof(bool));
    }
    primes.p[primes.len] = v;
    primes.used[primes.len] = false;
    ++primes.len;
}

static void grow_primes(void) {
    uint64_t start = primes.last + 1;
    uint64_t end   = primes.last + (primes.last > 1 ? primes.last : 1);
    if (end < start) end = start + 1000;
    for (uint64_t x = start; x <= end; ++x) {
        if (is_prime64(x)) add_prime(x);
    }
    primes.last = end;
}

static void ensure_at_least_9(void) {
    while (primes.len < 9) grow_primes();
}

/* ---------- digit mask for a value ---------- */
static inline uint16_t digit_mask(uint64_t v) {
    uint16_t m = 0;
    if (v == 0) return 1;
    while (v) {
        m |= 1 << (v % 10);
        v /= 10;
    }
    return m;
}

/* ---------- storage of sequence values ---------- */
struct TermList {
    seq_t *t;
    idx_t len;
    size_t cap;
} seq = {NULL, 0, 0};

static void add_term(seq_t val) {
    if (seq.len == seq.cap) {
        seq.cap = seq.cap ? seq.cap << 1 : 16;
        seq.t   = (seq_t*)realloc(seq.t, seq.cap * sizeof(seq_t));
    }
    seq.t[seq.len++] = val;
}

/* ---------- main function a() ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* unspecified for n < 1 */

    if (!seq.t) { /* first call */
        seq.t   = (seq_t*)malloc(16 * sizeof(seq_t));
        seq.cap = 16;
        seq.len = 0;
        ensure_prime_list();
    }

    while ((idx_t)seq.len < n) {
        seq_t term;
        if (seq.len == 0) { /* a(1) = prime(9) */
            ensure_at_least_9();
            term = (seq_t)primes.p[8];
            primes.used[8] = true;
        } else {
            seq_t prev = seq.t[seq.len - 1];
            uint16_t mask = digit_mask((uint64_t)prev);
            bool found = false;
            /* search existing primes */
            for (size_t i = 0; i < primes.len && !found; ++i) {
                if (!primes.used[i] && (digit_mask(primes.p[i]) & mask)) {
                    term = (seq_t)primes.p[i];
                    primes.used[i] = true;
                    found = true;
                }
            }
            /* if not found, generate more primes and retry */
            while (!found) {
                grow_primes();
                for (size_t i = 0; i < primes.len && !found; ++i) {
                    if (!primes.used[i] && (digit_mask(primes.p[i]) & mask)) {
                        term = (seq_t)primes.p[i];
                        primes.used[i] = true;
                        found = true;
                    }
                }
            }
        }
        add_term(term);
    }
    return seq.t[n - 1];
}
