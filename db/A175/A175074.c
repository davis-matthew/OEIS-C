/*
 * A175074  Nonprimes b with result 1 under iterations of {r mod (max prime p
 *          < r)} starting at r = b.
 * https://oeis.org/A175074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth non‑prime b such that iterating r←r mod (largest
 *            prime < r) eventually yields 1. The sequence contains 1 and all
 *            non‑prime b that reduce to 1 by this process
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* ---------- Prime sieve ---------- */
#define SIEVE_LIMIT 2000000ULL            /* sufficient for n≈10000 */

static bool primes_ready = false;
static uint32_t *prime_list   = NULL; /* primes up to SIEVE_LIMIT */
static size_t   prime_cnt      = 0;
static uint64_t prime_limit    = SIEVE_LIMIT;

/* allocate and fill prime_list for numbers up to SIEVE_LIMIT */
static void init_primes(void)
{
    if (primes_ready) return;
    /* temporary sieve array of bits */
    uint8_t *sieve = (uint8_t*)malloc(SIEVE_LIMIT + 1);
    if (!sieve) return;                       /* allocation failed */
    /* 0 = prime, 1 = composite */
    memset(sieve, 0, SIEVE_LIMIT + 1);
    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 1;
        }
    }
    /* estimate number of primes to allocate space */
    size_t approx = 200000;                    /* generous upper bound */
    prime_list = (uint32_t*)malloc(approx * sizeof(uint32_t));
    if (!prime_list) { free(sieve); return; }
    size_t idx = 0;
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            if (idx < approx) prime_list[idx++] = (uint32_t)i;
        }
    }
    prime_cnt = idx;
    free(sieve);
    primes_ready = true;
}

/* binary search: largest prime < x (x>2), return 0 if none */
static uint32_t prev_prime_lt(uint64_t x)
{
    if (x <= 2) return 0;
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < x) lo = mid + 1;
        else                     hi = mid;
    }
    if (lo == 0) return 0;
    return prime_list[lo - 1];
}

/* ---------- Sequence storage ---------- */
static seq_t *terms          = NULL; /* cached values of a(n) */
static size_t term_cnt       = 0;    /* how many have been found */
static size_t term_cap       = 0;    /* allocated capacity */
static uint64_t next_candidate = 1;  /* next integer to test */

/* add a value to the terms array, growing storage if needed */
static void add_term(seq_t val)
{
    if (term_cnt == term_cap) {
        size_t new_cap = (term_cap == 0) ? 16 : term_cap * 2;
        seq_t *new_arr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) return;                /* silently ignore allocation failure */
        terms = new_arr;
        term_cap = new_cap;
    }
    terms[term_cnt++] = val;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */

    init_primes();

    /* find terms until we have at least n of them */
    while (term_cnt < (size_t)n) {
        uint64_t cand   = next_candidate++;
        bool good = false;

        if (cand == 1) {
            good = true;                     /* 1 is explicitly good */
        } else if (!is_prime(cand)) {
            /* perform the iteration */
            uint64_t r = cand;
            for (;;) {
                if (r == 1) {               /* success */
                    good = true; break;
                }
                uint32_t p = prev_prime_lt(r);
                if (p == 0) {               /* no smaller prime, stuck */
                    good = false; break;
                }
                r %= p;
                if (r == 2) {               /* will never reach 1 */
                    good = false; break;
                }
            }
        } else {
            good = false;                    /* primes are not counted */
        }

        if (good) add_term((seq_t)cand);
    }

    return terms[n - 1];
}
