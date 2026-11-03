/*
 * A046502  Primes with multiplicative persistence value 2.
 * https://oeis.org/A046502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9957 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime whose multiplicative persistence equals 2 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_product.h"
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

/* Helper: true if p is a prime whose multiplicative persistence is exactly 2 */
static bool is_persistence2(seq_t p)
{
    seq_t prod1 = digit_product(p);
    if (prod1 <= 9) return false;        /* persistence 0 or 1 */
    seq_t prod2 = digit_product(prod1);
    return prod2 <= 9;                   /* persistence 2 */
}

/* Return the n‑th term (n≥1) of OEIS sequence A046502 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* defensive, undefined for n<1 */

    /* static state */
    static seq_t *primes = NULL;    /* array of primes with persistence 2 */
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t next_candidate = 2;

    /* initialise array lazily */
    if (primes == NULL) {
        primes_cap = 64;
        primes = mem_new(seq_t, primes_cap);
        if (!primes) exit(EXIT_FAILURE);
    }

    /* extend list until we have at least n primes */
    while (primes_len < n) {
        /* skip even numbers greater than 2 */
        if (next_candidate > 2 && (next_candidate & 1) == 0) {
            ++next_candidate;
            continue;
        }

        /* test primality */
        if (is_prime_big((seq_t)next_candidate)) {
            if (is_persistence2((seq_t)next_candidate)) {
                /* store prime */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                    if (!tmp) exit(EXIT_FAILURE);
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return primes[n - 1];
}
