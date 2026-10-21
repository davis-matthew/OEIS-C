/*
 * A091267  Lengths of runs of 3's in A039702.
 * https://oeis.org/A091267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   lengths of runs of primes ≡ 3 (mod 4) terminated by a prime ≡ 1
 *            (mod 4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- prime utilities ---------- */

static uint64_t *primes = NULL;
static size_t primes_len   = 0;
static size_t primes_cap   = 0;

static void grow_primes(void) {
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_cap;
}

static void add_prime(uint64_t p) {
    if (primes_len == primes_cap) grow_primes();
    primes[primes_len++] = p;
}

static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;           /* p*p would overflow or exceed n */
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- sequence computation ---------- */

static seq_t *runs = NULL;
static size_t runs_len = 0;
static size_t runs_cap = 0;
static seq_t current_run = 0;
static uint64_t last_checked = 2;   /* last prime examined */

static void grow_runs(void) {
    size_t new_cap = runs_cap ? runs_cap * 2 : 64;
    seq_t *tmp = (seq_t*)realloc(runs, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    runs = tmp;
    runs_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* safety for out‑of‑range calls */

    /* generate runs until we have at least n terms */
    while (runs_len < (size_t)n) {
        uint64_t p = last_checked + 1;
        /* find next prime */
        while (!is_prime_uint64(p)) ++p;
        last_checked = p;
        add_prime(p);

        unsigned rem = (unsigned)(p & 3);   /* remainder mod 4 */
        if (rem == 3) {
            current_run++;
        } else if (rem == 1) {
            if (current_run > 0) {
                if (runs_len == runs_cap) grow_runs();
                runs[runs_len++] = current_run;
                current_run = 0;
            }
            /* otherwise ignore lone 1‑mod‑4 prime */
        }
        /* other remainders cannot occur for primes > 2 */
    }

    return runs[n - 1];
}
