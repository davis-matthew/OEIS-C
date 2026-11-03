/*
 * A154966  Primes p such that the concatenation of p and prime(p) is
 *          composite.
 * https://oeis.org/A154966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = the n‑th prime p such that the decimal concatenation of p
 *            and the p‑th prime is composite
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 965 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static bool initialized = false;

/* Primes list */
static uint64_t *prime_list = NULL;  /* array of primes */
static size_t    prime_cnt   = 0;    /* number of primes stored */
static idx_t     prime_limit = 5000000ULL;  /* current sieve limit */

/* Cached good primes */
static seq_t *good_primes = NULL;
static size_t good_len    = 0;
static size_t good_cap    = 0;

/* Index of next prime in prime_list to test */
static idx_t next_test_idx = 0;

/* Ensure that prime_list contains at least 'need' primes. */
static void ensure_primes(idx_t need)
{
    while (prime_cnt < (size_t)need) {
        prime_limit *= 2ULL;
        free(prime_list);
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* below offset – undefined behavior allowed */
        return 0;

    if (!initialized) {
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);
        initialized = true;
    }

    while (good_len < n) {
        /* Make sure we still have an unexplored prime. */
        if (next_test_idx >= prime_cnt) {
            prime_limit *= 2ULL;
            free(prime_list);
            build_primes_upto(prime_limit, &prime_list, &prime_cnt);
        }

        uint64_t p = prime_list[next_test_idx++];
        /* Ensure we can access the p‑th prime (1‑based). */
        ensure_primes((idx_t)p);
        uint64_t prime_of_p = prime_list[(size_t)p - 1];

        seq_t concat_val = concat_parts((idx_t)p, (idx_t)prime_of_p);
        if (!is_prime_big(concat_val))
            append_candidate(&good_primes, &good_len, &good_cap, (seq_t)p);
    }

    return good_primes[n - 1];
}
