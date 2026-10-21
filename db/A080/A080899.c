/*
 * A080899  Primes p(k) such that every composite number between p(k) and
 *          p(k+1) has exactly three distinct prime factors.
 * https://oeis.org/A080899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 464, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080899: primes p such that every composite number
   between p and the next prime has exactly three distinct prime
   factors.  The sequence starts: 29, 41, 59, 101, 137, … */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* The implementation uses a linear sieve to pre‑compute the smallest
   prime factor for each integer up to MAX_LIMIT.  The first good
   prime, 29, occurs well before 10 000 000, and 10 M guarantees that
   the first 10 000 terms of the sequence are inside the limit. */

#define MAX_LIMIT 10000000U      /* adjust if more terms are needed */

/* --- static data ---------------------------------------------------- */
static uint32_t *spf           = NULL;  /* smallest prime factor array   */
static uint32_t *prime_list    = NULL;  /* list of primes                */
static size_t    prime_count   = 0;      /* number of computed primes     */
static size_t    proc_prime_idx = 0;     /* last processed prime index    */

static seq_t   *seq_arr        = NULL;  /* stored sequence terms         */
static size_t   seq_len        = 0;      /* number of terms stored        */
static size_t   seq_cap        = 0;      /* capacity of seq_arr           */

/* --- helpers -------------------------------------------------------- */
/* Build the linear sieve once.  After this function, spf[i] == i
   iff i is prime, otherwise the least prime factor of i. */
static void init_sieve(void)
{
    if (spf) return;                  /* already built */

    spf = (uint32_t *)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1);                /* OOM check */

    size_t cap = MAX_LIMIT / 10;       /* rough estimate of #primes   */
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_list) exit(1);

    size_t len = 0;
    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            spf[i] = i;
            if (len == cap) {
                cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
                if (!prime_list) exit(1);
            }
            prime_list[len++] = i;
        }
        for (size_t j = 0; j < len &&
             prime_list[j] <= spf[i] &&
             (uint64_t)i * prime_list[j] <= MAX_LIMIT; ++j) {
            spf[i * prime_list[j]] = prime_list[j];
        }
    }
    prime_count = len;
}

/* Test whether a prime p with its successor q satisfies the property.
   The interval must contain at least one composite number;
   otherwise the condition is vacuously true and p is not in the sequence. */
static bool good_prime(uint32_t p, uint32_t q)
{
    if (q <= p + 1) return false;   /* no composite in the gap */

    for (uint32_t x = p + 1; x < q; ++x) {
        if (spf[x] == x) continue;  /* skip primes */
        uint32_t y = x;
        int cnt = 0;
        while (y > 1) {
            uint32_t d = spf[y];
            ++cnt;
            if (cnt > 3) return false;         /* early exit */
            while (y % d == 0) y /= d;
        }
        if (cnt != 3) return false;
    }
    return true;
}

/* Grow the array that stores the sequence values. */
static void grow_seq(void)
{
    if (seq_cap == 0) seq_cap = 16;
    else seq_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    if (!seq_arr) exit(1);
}

/* Ensure that the sequence contains at least n terms. */
static void build_seq_upto(idx_t n)
{
    init_sieve();

    if (!seq_arr) {
        seq_cap = 16;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) exit(1);
    }

    while (seq_len < (size_t)n && proc_prime_idx + 1 < prime_count) {
        uint32_t p = prime_list[proc_prime_idx];
        uint32_t q = prime_list[proc_prime_idx + 1];
        if (good_prime(p, q)) {
            if (seq_len == seq_cap) grow_seq();
            seq_arr[seq_len++] = (seq_t)p;
        }
        ++proc_prime_idx;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the problem statement.                */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* offset is 1 */
    build_seq_upto(n);
    if ((size_t)(n - 1) < seq_len) {
        return seq_arr[n - 1];
    }
    /* If we reach this point the initialized limit is too small.  The
       sequence is not defined beyond the precomputed range under the
       performance constraints.  Return 0 as a sentinel. */
    return 0;
}
