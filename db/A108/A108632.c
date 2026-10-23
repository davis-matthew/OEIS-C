/*
 * A108632  Semiprimes with prime digits (only digits 2,3,5,7 in semiprimes).
 * https://oeis.org/A108632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 282, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generates semiprimes whose decimal digits are only
   2,3,5,7.  The numbers are produced in increasing order and tested
   for semiprimality by trial division using a prime table up to
   1 000 000. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* prime table up to LIMIT */
#define PRIME_LIMIT 1000000
static uint32_t primes[78500];
static int32_t primes_cnt = 0;

/* powers of 4 for length handling */
static uint32_t pow4_tbl[20];

/* digit mapping for base‑4 encoding */
static const uint8_t digit_map[4] = { 2, 3, 5, 7 };

/* storage for generated sequence terms */
static seq_t *term_arr = NULL;
static size_t term_sz  = 0;
static size_t term_cap = 0;

/* current state for candidate enumeration */
static uint32_t curr_len  = 2;   /* start with 2‑digit numbers */
static uint32_t idx_in_len = 0;  /* index within current length */

/* initialise primes and tables */
static void init_tables(void) {
    static bool init_done = false;
    if (init_done) return;
    init_done = true;

    /* sieve for primes up to PRIME_LIMIT */
    bool *sieve = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    memset(sieve, 1, (PRIME_LIMIT + 1) * sizeof(bool));
    sieve[0] = sieve[1] = false;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    primes_cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[primes_cnt++] = i;
    free(sieve);

    /* precompute powers of 4 */
    pow4_tbl[0] = 1;
    for (int i = 1; i < 20; ++i)
        pow4_tbl[i] = pow4_tbl[i - 1] << 2;  /* multiply by 4 */
}

/* simple primality test using precomputed primes */
static bool isPrime(uint64_t x) {
    if (x < 2) return false;
    for (int i = 0; i < primes_cnt &&
             (uint64_t)primes[i] * primes[i] <= x; ++i) {
        if (x % primes[i] == 0) return false;
    }
    return true;
}

/* test whether x is a semiprime (product of exactly two primes) */
static bool isSemiprime(uint64_t x) {
    if (isPrime(x)) return false;          /* not composite */
    for (int i = 0; i < primes_cnt &&
             (uint64_t)primes[i] * primes[i] <= x; ++i) {
        if (x % primes[i] == 0) {
            uint64_t q = x / primes[i];
            return isPrime(q);              /* ensure q is prime */
        }
    }
    return false;
}

/* produce the next candidate number with only prime digits */
static uint64_t next_candidate(void) {
    uint32_t d = curr_len;
    uint32_t idx = idx_in_len;
    uint8_t digits[20];           /* enough for any realistic length */
    uint32_t temp = idx;

    /* convert idx in base‑4 to digits */
    for (uint32_t pos = 0; pos < d; ++pos) {
        digits[pos] = digit_map[temp & 3];
        temp >>= 2;                /* equivalent to temp /= 4 */
    }

    /* build integer value from most significant digit first */
    uint64_t val = 0;
    for (int pos = d - 1; pos >= 0; --pos)
        val = val * 10u + digits[pos];

    /* advance to next candidate */
    ++idx_in_len;
    if (idx_in_len == pow4_tbl[d]) {
        curr_len++;
        idx_in_len = 0;
    }

    return val;
}

/* ensure term_arr has room for n terms and generate missing ones */
static void ensure_n_terms(idx_t n) {
    while (term_sz < (size_t)n) {
        uint64_t cand = next_candidate();
        if (isSemiprime(cand)) {
            seq_t v = (seq_t)cand;
            if (term_sz >= term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 32;
                seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (term_arr && term_sz)
                    memcpy(new_arr, term_arr, term_sz * sizeof(seq_t));
                if (term_arr) free(term_arr);
                term_arr = new_arr;
                term_cap = new_cap;
            }
            term_arr[term_sz++] = v;
        }
    }
}

/* --- required sequence function --------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    init_tables();
    ensure_n_terms(n);
    return term_arr[n - 1];
}
