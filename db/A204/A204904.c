/*
 * A204904  p(n)-q(n), where (p(n), q(n)) is the least pair of odd primes for
 *          which n divides p(n)-q(n).
 * https://oeis.org/A204904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = least even multiple of n that is a difference of two odd primes. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
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

static uint64_t *prime_list = NULL;       /* array of primes up to prime_limit */
static size_t      prime_cnt = 0;         /* number of primes in prime_list */
static unsigned char *prime_sieve = NULL; /* boolean sieve: 1 if prime */
static idx_t         prime_limit = 0;     /* current upper bound for primes */

/* Rebuild the prime list and sieve up to at least 'needed'. */
static void ensure_prime_limit(idx_t needed) {
    if (prime_limit >= needed) return;
    /* choose a new limit, doubling until sufficient */
    idx_t new_limit = prime_limit ? prime_limit : 2000000;   /* start with 2 million */
    while (new_limit < needed) new_limit <<= 1;              /* double until big enough */

    /* free old structures */
    free(prime_list);
    free(prime_sieve);

    /* build new prime list */
    build_primes_upto(new_limit, &prime_list, &prime_cnt);

    /* build sieve for quick lookup */
    prime_sieve = (unsigned char *)malloc((size_t)(new_limit + 1));
    memset(prime_sieve, 0, (size_t)(new_limit + 1));
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p <= new_limit) prime_sieve[(size_t)p] = 1;
    }

    prime_limit = new_limit;
}

/* Assume prime_list is always initialised before first use. */
static void init_primes(void) {
    if (prime_list == NULL) {
        ensure_prime_limit(2000000);          /* start with a generous bound */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined below offset, but return 0 for safety */

    init_primes();                    /* make sure primes are ready */

    /* start with first multiplier giving an even product with n */
    idx_t t = (n % 2 == 0) ? 1 : 2;
    for (;;) {
        seq_t m = (seq_t)n * (seq_t)t;   /* candidate difference, guaranteed even */

        /* ensure our prime list is large enough to test up to at least m + 3 */
        idx_t required_limit = (idx_t)(m + 3);
        if ((idx_t)m > prime_limit) {
            ensure_prime_limit(required_limit);
        }

        /* scan odd primes q, looking for q + m that is prime */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q_ull = prime_list[i];
            if (q_ull == 2) continue;                   /* skip even prime 2 */
            seq_t q = (seq_t)q_ull;
            seq_t p = q + m;
            bool p_is_prime;
            if (p <= (seq_t)prime_limit) {
                p_is_prime = prime_sieve[(size_t)p];
            } else {
                p_is_prime = is_prime_big(p);
            }
            if (p_is_prime) {
                return m;   /* found minimal difference divisible by n */
            }
        }

        /* if we reached here no pair was found for this m,
           try next even multiple of n */
        ++t;
    }
}
