/*
 * A089785  a(n) is the least prime(k) such that the concatenation of prime(k)
 *          and prime(k-n) is prime.
 * https://oeis.org/A089785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest prime p = prime(k) for k ≥ n+1 such that
 *            concat(prime(k), prime(k−n)) is prime. The index k is 1‑based
 * Range:     u8 n <= 8, u16 n <= 222, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9154 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
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

/* Prime table */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt   = 0;
static bool prime_init    = false;

/* Initialise the prime table up to a fixed limit (2 000 000). */
static void init_primes(void)
{
    if (prime_init)
        return;
    prime_init = true;

    const idx_t LIMIT = 2000000ULL;
    /* Allocate and build primes */
    build_primes_upto(LIMIT, &prime_tbl, &prime_cnt);
    /* If the build failed, prime_tbl will be NULL; callers will return 0. */
}

/* Primary function computing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1; safe guard */

    init_primes();
    if (prime_tbl == NULL)
        return 0;                    /* failed to initialise */

    /* Search for the smallest k ≥ n+1 such that the concatenation is prime. */
    for (idx_t k = n + 1; k <= prime_cnt; ++k) {
        /* Indices are 1‑based; array indices are 0‑based. */
        uint64_t pk   = prime_tbl[k - 1];
        uint64_t pk_n = prime_tbl[(k - n) - 1];

        /* Concatenate the two primes; both arguments fit in idx_t. */
        seq_t cat = concat_parts((idx_t)pk, (idx_t)pk_n);

        if (is_prime_big(cat)) {
            return (seq_t)pk;        /* cast to seq_t for the result type */
        }
    }

    /* If we run out of primes (should not happen for n ≤ 10000 with the
       chosen LIMIT), return 0 as a fallback. */
    return 0;
}
