/*
 * A144919  Duplicate of A007645.
 * https://oeis.org/A144919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 1 (mod 6), with 3 as the first element
   a(1) = 3
   for n ≥ 2, a(n) is the (n‑1)th prime p satisfying p ≡ 1 (mod 6).  */

/* The implementation keeps two simple dynamic arrays: one of all primes
   found so far, used for primality testing, and one of the sequence
   values.  No external libraries are used.  */
#include <stddef.h>
#include <stdlib.h>

/* ---- global state for primes and the sequence values ---- */
static idx_t   *primes          = NULL;  /* all primes discovered */
static size_t  primes_cap      = 0;
static size_t  primes_len      = 0;

static seq_t   *seq_vals        = NULL;  /* 3, 7, 13, ... */
static size_t  seq_cap         = 0;
static size_t  seq_len         = 0;

static idx_t next_candidate = 2;        /* next integer to test for primality */

/* ---- helpers to grow the prime and sequence arrays ---- */
static void grow_primes(void)
{
    size_t new_cap = primes_cap ? primes_cap * 2 : 64;
    idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

static void add_prime(idx_t p)
{
    if (primes_len == primes_cap) grow_primes();
    primes[primes_len++] = p;
}

static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_vals = tmp;
    seq_cap = new_cap;
}

static void add_seq_val(seq_t v)
{
    if (seq_len == seq_cap) grow_seq();
    seq_vals[seq_len++] = v;
}

/* ---- primality test using the stored prime list ---- */
static int is_prime(idx_t n)
{
    if (n < 2) return 0;
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* ---- make sure seq_vals contains at least (needed+1) elements ----
   (needed is a zero‑based index) ---- */
static void ensure_seq(size_t needed)
{
    if (needed < seq_len) return;          /* already satisfied */

    /* First call: initialise the prime list and the first sequence value */
    if (seq_len == 0) {
        add_prime(2);          /* 2 is a prime but not in the sequence */
        add_prime(3);          /* 3 is the first sequence value */
        add_seq_val((seq_t)3);
        next_candidate = 4;
    }

    /* Generate primes until we have enough sequence values */
    while (seq_len <= needed) {
        /* Skip even numbers after 2 */
        if (next_candidate % 2 == 0) {
            ++next_candidate;
            continue;
        }

        if (is_prime(next_candidate)) {
            add_prime(next_candidate);
            if (next_candidate % 6 == 1) {
                add_seq_val((seq_t)next_candidate);
            }
        }
        ++next_candidate;
    }
}

/* ---- public function computing a(n) ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, so a(0) is undefined */
    size_t needed = (size_t)n - 1;     /* zero‑based index for n */
    ensure_seq(needed);
    return seq_vals[needed];
}
