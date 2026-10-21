/*
 * A094045  Alternate composite and prime numbers not included earlier such
 *          that every concatenation of a pair of terms is a prime: a(2n) is
 *          prime and a(2n-1) is nonprime.
 * https://oeis.org/A094045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   The sequence is built iteratively. a(1)=1. For n>1, a(n) is the
 *            smallest positive integer not already in the sequence such that
 *            • n even → a(n) is prime, • n odd → a(n) is composite, • the
 *            decimal concatenation a(n‑1) a(n) is prime
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers needed for the helper functions */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------- */
/*   Global state (static to this translation unit)                     */
/* -------------------------------------------------------------------- */
static seq_t *terms   = NULL;   /* dynamic array of produced terms          */
static size_t terms_len = 0, terms_cap = 0;   /* size / capacity of terms array     */
static bool  *used    = NULL;   /* bitmap of numbers already in the sequence */
static size_t used_cap  = 0;     /* capacity of the bitmap                */

/* -------------------------------------------------------------------- */
/*  Prime testing utilities                                           */
/* -------------------------------------------------------------------- */

/* 1‑digit primes for quick modular checks                            */
static const uint32_t SMALL_PRIMES[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97
};

/* Multiply modulo 64‑bit avoiding overflow                           */
static inline uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Binary exponentiation modulo 64‑bit                                  */
static uint64_t powmod_u64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod_u64(r, a, mod);
        a = mulmod_u64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers                       */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]); ++i) {
        uint32_t p = SMALL_PRIMES[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod_u64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool witness = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod_u64(x, x, n);
            if (x == n-1) { witness = false; break; }
        }
        if (witness) return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */
/*  Miscellaneous helpers                                            */
/* -------------------------------------------------------------------- */

/* Count decimal digits of a positive 64‑bit integer.  Return at least 1. */
static unsigned int digits_u64(uint64_t x)
{
    unsigned int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/* 10^k for k=0..20 (fits 128‑bit)                                        */
static const unsigned __int128 POW10[21] = {
     (unsigned __int128)1ULL,
     (unsigned __int128)10ULL,
     (unsigned __int128)100ULL,
     (unsigned __int128)1000ULL,
     (unsigned __int128)10000ULL,
     (unsigned __int128)100000ULL,
     (unsigned __int128)1000000ULL,
     (unsigned __int128)10000000ULL,
     (unsigned __int128)100000000ULL,
     (unsigned __int128)1000000000ULL,
     (unsigned __int128)10000000000ULL,
     (unsigned __int128)100000000000ULL,
     (unsigned __int128)1000000000000ULL,
     (unsigned __int128)10000000000000ULL,
     (unsigned __int128)100000000000000ULL,
     (unsigned __int128)1000000000000000ULL,
     (unsigned __int128)10000000000000000ULL,
     (unsigned __int128)100000000000000000ULL,
     (unsigned __int128)1000000000000000000ULL,
     (unsigned __int128)10000000000000000000ULL
};

/* -------------------------------------------------------------------- */
/*  Bitmap helpers (used numbers)                                    */
/* -------------------------------------------------------------------- */
static void ensure_used(uint64_t max_needed)
{
    if (max_needed < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= (size_t)max_needed) new_cap <<= 1;
    bool *new_bits = (bool*)realloc(used, new_cap);
    memset(new_bits + used_cap, 0, new_cap - used_cap);
    used = new_bits;
    used_cap = new_cap;
}

static void mark_used(uint64_t v)
{
    ensure_used(v);
    used[v] = true;
}

static bool is_used(uint64_t v)
{
    return v < used_cap && used[v];
}

/* -------------------------------------------------------------------- */
/*  Terms array helpers                                                */
/* -------------------------------------------------------------------- */
static void ensure_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* -------------------------------------------------------------------- */
/*  Generate the next term in the sequence (append it to terms array). */
/* -------------------------------------------------------------------- */
static void generate_next(void)
{
    uint64_t prev = (uint64_t)terms[terms_len - 1];
    bool need_prime = ((terms_len + 1) % 2 == 0);  /* even indices → prime */
    uint64_t candidate = 2;

    while (1) {
        if (is_used(candidate)) { ++candidate; continue; }
        if (need_prime != is_prime_u64(candidate)) { ++candidate; continue; }

        unsigned d = digits_u64(candidate);
        unsigned __int128 concat = (unsigned __int128)prev * POW10[d] + candidate;
        if (concat > UINT64_MAX) { ++candidate; continue; }
        if (!is_prime_u64((uint64_t)concat)) { ++candidate; continue; }

        /* Found a valid candidate. */
        mark_used(candidate);
        ensure_terms(terms_len + 1);
        terms[terms_len++] = (seq_t)candidate;
        return;
    }
}

/* -------------------------------------------------------------------- */
/*  Public function requested by the framework                          */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for non‑positive indices */

    /* Initialise on first call. */
    if (!terms) {
        terms    = (seq_t*)malloc(16 * sizeof(seq_t));
        used     = (bool*)malloc(16 * sizeof(bool));
        memset(used, 0, 16 * sizeof(bool));
        terms_cap = used_cap = 16;
        terms_len = 1;
        terms[0] = (seq_t)1;
        mark_used(1);
    }

    /* Grow the sequence until it contains the requested index. */
    while (terms_len < (size_t)n) generate_next();
    return terms[n - 1];
}
