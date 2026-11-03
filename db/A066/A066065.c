/*
 * A066065  a(n) = smallest prime q such that in decimal notation the
 *          concatenation prime(n)q yields a prime ( = A066064(n)).
 * https://oeis.org/A066065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = min{q : q is prime and concat(prime(n), q) is prime}
 * Range:     u8 n <= 10, u16 n <= 1240, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 105 terms through n = 9964 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 3, a(2) = 7, a(3) = 3, a(4) = 3, ... as in OEIS A066065 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
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

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑indexed).  get_prime returns a 32‑bit value. */
    uint32_t p = get_prime(n);

    /* Lazily build a list of candidate primes q. */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;

    if (prime_list == NULL) {
        /* 2 000 000 is more than enough for a(n) < prime(n) for n≤10 000. */
        const idx_t LIMIT = 2000000;
        build_primes_upto(LIMIT, &prime_list, &prime_count);
    }

    /* Search for the smallest q such that concat(p,q) is prime. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        seq_t candidate = concat_parts((idx_t)p, (idx_t)q);
        if (is_prime_big(candidate)) {
            return (seq_t)q;
        }
    }

    /* In practice the previous loop always finds an answer.  If it does not,
       extend the prime table and continue searching (unlikely for n≤10 000). */
    while (1) {
        /* double the limit each time */
        const idx_t new_limit = prime_count ? prime_count * 2 : 2000000;
        build_primes_upto(new_limit, &prime_list, &prime_count);

        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_list[i];
            seq_t candidate = concat_parts((idx_t)p, (idx_t)q);
            if (is_prime_big(candidate)) {
                return (seq_t)q;
            }
        }
    }
}
