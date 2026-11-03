/*
 * A176134  Primes p of the form |prime(n+2)^2-prime(n+1)^2-prime(n)^2|,
 *          (absolute values).
 * https://oeis.org/A176134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A176134: a(n) = prime of the form |prime(n+2)^2 - prime(n+1)^2 - prime(n)^2|
 * Only the number itself is prime after the absolute value. The sequence is offset 1.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *seq_values = NULL;     /* array of computed sequence terms */
static size_t seq_cap   = 0;         /* current capacity of seq_values      */
static idx_t seq_len    = 0;         /* number of sequence terms stored */

static uint64_t *prime_arr = NULL;  /* array of first primes */
static size_t prime_cap   = 0;       /* current capacity of prime_arr   */
static idx_t prime_len    = 0;       /* number of primes stored         */

static idx_t next_candidate = 0;     /* index n of the next triple to test */

/* Ensure seq_values has room for 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_values = mem_resize(seq_t, seq_values, new_cap);
    seq_cap    = new_cap;
}

/* Ensure prime_arr has room for 'need' primes */
static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    prime_arr = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    prime_cap = new_cap;
}

/* Initialise the first three primes */
static void init_primes(void)
{
    if (prime_len == 0) {
        ensure_prime_cap(3);
        prime_arr[0] = 2;
        prime_arr[1] = 3;
        prime_arr[2] = 5;
        prime_len    = 3;
    }
}

/* Generate the next prime and store it */
static void generate_next_prime(void)
{
    uint64_t next = next_prime(prime_arr[prime_len - 1] + 1);
    ensure_prime_cap(prime_len + 1);
    prime_arr[prime_len++] = next;
}

/* Compute sequence terms until we have at least 'n' terms */
static void compute_terms_up_to(idx_t n)
{
    init_primes();
    while (seq_len < n) {
        /* Ensure we have enough primes for the current triple */
        while (prime_len < next_candidate + 3) {
            generate_next_prime();
        }

        uint64_t p0 = prime_arr[next_candidate];
        uint64_t p1 = prime_arr[next_candidate + 1];
        uint64_t p2 = prime_arr[next_candidate + 2];

        /* Use 128‑bit arithmetic to avoid overflow */
        signed __int128 diff128 = (signed __int128)p2 * p2
                               - (signed __int128)p1 * p1
                               - (signed __int128)p0 * p0;
        if (diff128 < 0) diff128 = -diff128;

        /* Only store if the resulting value is prime */
        if (is_prime_big((seq_t)diff128)) {
            ensure_seq_cap(seq_len + 1);
            seq_values[seq_len++] = (seq_t)diff128;
        }

        /* Move to the next triple, regardless of whether we stored a value */
        next_candidate++;
    }
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset, harmless */
    if (seq_len < n) compute_terms_up_to(n);
    return seq_values[n - 1];              /* offset 1: a(1) is the first term */
}
