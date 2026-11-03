/*
 * A045367  Primes congruent to {1, 2, 5} mod 7.
 * https://oeis.org/A045367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 974 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: incremental search for primes ≡ 1, 2, 5 (mod 7) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Cached primes of the desired residue classes */
static seq_t *primes = NULL;   /* array of primes */
static size_t primes_len = 0;  /* number of primes generated */
static size_t primes_cap = 0;  /* allocated capacity of the array */

/* Return the n-th prime that is ≡ 1, 2, or 5 (mod 7). */
seq_t a(idx_t n)
{
    /* OEIS offset is 1; n == 0 is undefined but we return 0 */
    if (n == 0) return 0;

    /* Generate more primes if necessary. */
    while (primes_len < (size_t)n) {
        seq_t cand;
        if (primes_len == 0)
            cand = 2;                     /* first prime candidate */
        else
            cand = primes[primes_len - 1] + 1;

        /* Search for the next qualifying prime. */
        for (;;) {
            int rem = (int)(cand % 7);
            if (rem != 1 && rem != 2 && rem != 5) {
                ++cand;                    /* skip unsuitable residue */
                continue;
            }

            if (is_prime_big(cand)) {
                /* Extend the cache if it is full. */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? (primes_cap << 1) : 8;
                    seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                    if (!tmp) abort();      /* allocation failure */
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = cand;   /* store the new prime */
                break;                         /* move on to generate next */
            }
            ++cand;                    /* try next integer */
        }
    }

    return primes[n - 1];
}
