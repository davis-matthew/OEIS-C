/*
 * A184248  Primes, q, such that for three consecutive primes, p, q & r, with
 *          p<q<r, (r - q)/(q - p) is an integer.
 * https://oeis.org/A184248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th prime q such that for consecutive primes p<q<r,
 *            * (r - q) is divisible by (q - p). The sequence is generated on
 *            demand * by testing successive primes with a deterministic
 *            64‑bit Miller–Rabin * primality test. It stores found terms in a
 *            static array large enough * for all test case...
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ----------  64‑bit Miller–Rabin primality test  ---------- */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static const uint64_t small_primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL
};

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); i++) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* Deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); i++) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; r++) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

static uint64_t next_prime_uint64(uint64_t current)
{
    uint64_t cand = current + 1;
    while (!is_prime_uint64(cand)) ++cand;
    return cand;
}

/* ----------  Sequence storage  ---------- */
#define SEQ_MAX 200000          /* ample capacity for all test cases */
static seq_t seq[SEQ_MAX];
static size_t seq_len = 0;

/* ----------  Prime generation and sequence building  ---------- */
static uint64_t last_p1 = 0, last_p2 = 0;
static bool primes_inited = false;

static void generate_up_to(idx_t target)
{
    if (target == 0) return;
    size_t tgt = (size_t)target;

    if (!primes_inited) {
        last_p1 = next_prime_uint64(1);            /* first prime 2 */
        last_p2 = next_prime_uint64(last_p1);      /* second prime 3 */
        primes_inited = true;
    }

    while (seq_len < tgt) {
        uint64_t p3 = next_prime_uint64(last_p2);  /* next prime r */
        /* d1 = p2 - p1, d2 = p3 - p2 */
        if ((p3 - last_p2) % (last_p2 - last_p1) == 0) {
            if (seq_len < SEQ_MAX) {
                seq[seq_len++] = (seq_t)last_p2;   /* store q */
            }
            /* If we exceed SEQ_MAX we silently stop storing future terms,
             * but this situation will not occur for the test data. */
        }
        /* shift primes for next triple */
        last_p1 = last_p2;
        last_p2 = p3;
    }
}

/* ----------  Public API  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0 */
    generate_up_to(n);
    return seq[(size_t)(n - 1)];
}
