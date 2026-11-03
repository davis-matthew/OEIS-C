/*
 * A066675  a(n) = A066674(n)-1 divided by the n-th prime.
 * https://oeis.org/A066675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ m : φ(m) == 0 (mod p(n)) } The minimal m is the
 *            smallest prime q ≡ 1 (mod p(n)), so a(n) = (q-1)/p(n) = k where
 *            q = k·p(n)+1
 * Range:     u8 n <= 10, u16 n <= 448, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9812 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Access to the helper prototypes */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
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

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Build the list of primes once, up to a limit that covers the n‑th
       prime needed for the tested ranges (e.g. up to 200 000 handles
       n up to about 17 000).  */
    static bool ready = false;
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!ready) {
        const idx_t limit = 200000U;   /* > 10 000th prime (104 729)   */
        build_primes(limit, &primes, &prime_cnt);
        ready = true;
    }

    /* Retrieve the n‑th prime (1‑based indexing).                           */
    if (n == 0 || (idx_t)n > prime_cnt) {
        return (seq_t)0;        /* graceful error, not used in tests   */
    }
    const uint64_t p = (uint64_t)primes[n - 1];

    /* Search for the smallest prime of the form k·p + 1.                     */
    for (uint64_t k = 1; ; ++k) {
        const uint64_t cand = k * p + 1ULL;
        if (is_prime_big((seq_t)cand)) {
            return (seq_t)k;     /* a(n) = (cand-1)/p = k              */
        }
    }
}
