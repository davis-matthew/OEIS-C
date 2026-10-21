/*
 * A089610  Number of primes between n^2 and (n+1/2)^2.
 * https://oeis.org/A089610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3787, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* Count primes p with n² < p ≤ n² + n  (OEIS A089610).               */
/* The function builds a sieve on demand, reusing static state.       */

/* ---------- Helpers for a bit array of odd numbers 3..limit --------- */
static inline bool get_bit(const unsigned char *bits, idx_t n)
{
    /* n is odd, ≥ 3 */
    idx_t i = (n - 3) >> 1;
    return (bits[i >> 3] >> (i & 7)) & 1;
}

static inline void set_bit(unsigned char *bits, idx_t n)
{
    /* n is odd, ≥ 3 */
    idx_t i = (n - 3) >> 1;
    bits[i >> 3] |= (1 << (i & 7));
}

/* ---------- Static state for the sieve -------------------------------- */
static idx_t sieve_limit = 0;          /* largest number sieved so far  */
static unsigned char *sieve_bits = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* ---------- Rebuild sieve to cover all numbers ≤ limit ---------------- */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* already ready           */

    /* Allocate bit array for odd numbers 3..limit: one bit per odd.    */
    size_t odd_count = (limit >= 3) ? (limit - 1) / 2 : 0;
    size_t bits_bytes = (odd_count + 7) >> 3;
    unsigned char *new_bits = (unsigned char*)calloc(bits_bytes, 1); /* all 0 → prime */

    /* Eratosthenes sieve on odd numbers.                           */
    idx_t root = (idx_t)floor(sqrt((long double)limit));
    for (idx_t p = 3; p <= root; p += 2) {
        if (!get_bit(new_bits, p)) {
            /* start at p*p, skip even multiples with step p*2. */
            for (idx_t q = p * p; q <= limit; q += p * 2) {
                set_bit(new_bits, q);
            }
        }
    }

    /* Build list of primes up to limit. */
    size_t est = (size_t)(limit / (log((double)limit) + 1e-9) * 1.2) + 100;
    uint32_t *new_primes = (uint32_t*)malloc(est * sizeof(uint32_t));
    size_t cnt = 0;
    if (limit >= 2) { new_primes[cnt++] = 2; }
    for (idx_t p = 3; p <= limit; p += 2) {
        if (!get_bit(new_bits, p)) {
            if (cnt >= est) {
                est = est * 2 + 10;
                new_primes = (uint32_t*)realloc(new_primes, est * sizeof(uint32_t));
            }
            new_primes[cnt++] = (uint32_t)p;
        }
    }

    /* Replace old data. */
    free(sieve_bits);
    free(prime_list);
    sieve_bits = new_bits;
    prime_list = new_primes;
    prime_cnt  = cnt;
    sieve_limit = limit;
}

/* ---------- Simple primality test using the sieve --------------------- */
static inline bool is_prime(idx_t n)
{
    if (n == 2) return true;
    if (n < 2 || (n & 1) == 0) return false;
    return !get_bit(sieve_bits, n);
}

/* ---------- Main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for off‑by‑offset callers */

    idx_t low  = (idx_t)n * (idx_t)n;      /* n²   */
    idx_t high = low + (idx_t)n;           /* n²+n = ⌊(n+½)²⌋ */

    build_sieve(high);        /* ensure all numbers up to high are sieved */

    seq_t count = 0;
    for (idx_t x = low + 1; x <= high; ++x) {
        if (is_prime(x)) ++count;
    }
    return count;
}
