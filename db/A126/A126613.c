/*
 * A126613  a(1)=1. a(n) = a(n-1) + (number of terms, from among terms a(1)
 *          through a(n-1), which are powers of primes {including 1}).
 * https://oeis.org/A126613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126613
   a(1) = 1
   a(n) = a(n-1) + (# of terms among a(1)..a(n-1) that are powers of primes,
                   including 1)
   The function keeps a memoised table; it can be called in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *seq_vals    = NULL;   /* 1‑based array of terms */
static uint8_t *prime_pow = NULL;   /* 1 if term is a prime power */
static size_t capacity   = 0;       /* allocated size of the arrays (≥1) */
static size_t last      = 0;        /* last index that has been computed */
static size_t count_pp  = 0;        /* number of prime powers seen so far */

/* -------------------------------------------------------------
   is_prime_power(v): return true if v is a power of a prime
   (including v <= 1, which are treated as prime powers).
   ------------------------------------------------------------- */
static bool is_prime_power(seq_t v)
{
    if (v <= 1) return true;    /* 0 and 1 are treated as prime powers */

    seq_t d = 0;
    /* trial division up to sqrt(v) */
    for (seq_t i = 2; i * i <= v; ++i) {
        if (v % i == 0) { d = i; break; }
    }
    if (d == 0) return true;    /* v itself is prime */

    /* divide out the found prime factor */
    while (v % d == 0) v /= d;
    return v == 1;
}

/* -------------------------------------------------------------
   ensure_capacity(n): make sure seq_vals and prime_pow have room
   for indices up to n (1‑based).  The arrays are resized by
   doubling whenever needed.  Newly allocated elements are
   initialised to 0.
   ------------------------------------------------------------- */
static void ensure_capacity(idx_t n)
{
    if (n <= (idx_t)capacity) return;

    size_t new_cap = (capacity == 0) ? 2 : capacity * 2;
    while (new_cap < (size_t)n) new_cap <<= 1;

    seq_t *new_seq = (seq_t *)realloc(seq_vals, (new_cap + 1) * sizeof(seq_t));
    uint8_t *new_pp = (uint8_t *)realloc(prime_pow,
                                         (new_cap + 1) * sizeof(uint8_t));
    if (!new_seq || !new_pp) abort();  /* allocation failure */

    seq_vals  = new_seq;
    prime_pow = new_pp;

    for (size_t i = capacity + 1; i <= new_cap; ++i) {
        seq_vals[i]  = (seq_t)0;
        prime_pow[i] = 0;
    }
    capacity = new_cap;
}

/* -------------------------------------------------------------
   compute_up_to(n): compute terms a(1)..a(n) if not already done.
   The function updates the global state accordingly.
   ------------------------------------------------------------- */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);
    if (last >= (size_t)n) return;   /* already computed */

    /* initialise a(1) if this is the very first call */
    if (last == 0) {
        seq_vals[1]   = (seq_t)1;
        prime_pow[1]  = 1;
        count_pp      = 1;
        last          = 1;
    }

    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_vals[i] = seq_vals[i - 1] + (seq_t)count_pp;
        bool pp = is_prime_power(seq_vals[i]);
        prime_pow[i] = pp ? 1 : 0;
        if (pp) ++count_pp;
    }
    last = (size_t)n;
}

/* -------------------------------------------------------------
   The required function: returns a(n).
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n==0, but safe guard */
    compute_up_to(n);
    return seq_vals[(size_t)n];
}
