/*
 * A099332  Primes p such that p = a^2 + b^2 for a,b>0 and a+b is prime.
 * https://oeis.org/A099332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that p = a² + b², a,b>0 and a+b is
 *            prime. Computation: generate primes by a sieve, test each prime
 *            until the n‑th term is found
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ---------- global state ---------- */
static bool initialized = false;
static uint8_t *is_prime = NULL;          /* bit array of primality up to INITIAL_LIMIT */
static const idx_t INITIAL_LIMIT = 20000000;  /* ≈20 M, enough for first 10 000 terms */
static idx_t current_prime_ptr = 0;       /* index of next prime to check */

static seq_t *seq_values = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* ---------- helpers ---------- */

/* Reallocate seq_values to hold at least `needed` elements */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? (seq_cap << 1) : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_values = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Build the sieve and fill is_prime array */
static void build_sieve(void)
{
    is_prime = (uint8_t *)calloc(INITIAL_LIMIT + 1, 1);
    if (!is_prime) exit(1);

    /* Mark every number >1 initially as prime */
    for (idx_t i = 2; i <= INITIAL_LIMIT; ++i) is_prime[i] = 1;

    idx_t sq = (idx_t) sqrt((double) INITIAL_LIMIT);
    for (idx_t i = 2; i <= sq; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= INITIAL_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }
}

/* Check whether a prime satisfies the definition */
static bool qualifies_prime(idx_t p)
{
    /* 2 = 1^2 + 1^2, a+b=2 prime */
    if (p == 2) return true;
    /* Only primes ≡1 (mod 4) can be written as sum of two squares */
    if ((p & 3) != 1) return false;

    idx_t max_a = (idx_t) sqrt((double)p / 2.0) + 1;
    for (idx_t a = 1; a <= max_a; ++a) {
        uint64_t a2 = (uint64_t)a * a;
        if (a2 > p) break;
        uint64_t b2 = (uint64_t)p - a2;
        uint64_t b = (uint64_t) (sqrt((double)b2) + 0.5);
        if (b > 0 && b * b == b2) {
            /* a+b must be prime; check within sieve */
            if (a + b <= INITIAL_LIMIT && is_prime[a + b]) return true;
        }
    }
    return false;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_sieve();
        seq_values = (seq_t *)malloc(128 * sizeof(seq_t));
        if (!seq_values) exit(1);
        seq_cap = 128;
        seq_len = 0;
        current_prime_ptr = 2;   /* start checking from 2 */
        initialized = true;
    }

    while (seq_len < n) {
        /* Find the next prime in the sieve */
        while (current_prime_ptr <= INITIAL_LIMIT && !is_prime[current_prime_ptr])
            ++current_prime_ptr;

        if (current_prime_ptr > INITIAL_LIMIT) return 0; /* exhausted */

        idx_t p = current_prime_ptr++;
        if (qualifies_prime(p)) {
            ensure_seq_capacity(seq_len + 1);
            seq_values[seq_len++] = (seq_t)p;
        }
    }

    return seq_values[n - 1];
}
