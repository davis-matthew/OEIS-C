/*
 * A129919  a(n) is the smallest prime of the form b*prime(n+1) + prime(n)
 *          with b > 0.
 * https://oeis.org/A129919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 222, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 990 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129919: a(n) = smallest prime of the form b*prime(n+1)+prime(n) with b>0
   (prime(1)=2).  This implementation generates primes on demand and tests
   the candidate primes using the is_prime_big() utility. */

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

static uint64_t *primes = NULL;   /* array of primes up to the current limit */
static size_t    prime_cnt = 0;   /* number of primes stored */
static idx_t     last_limit = 0;  /* last sieve upper bound used */

/* Ensure we have at least the (n+1)-st prime available. */
static void
ensure_primes(idx_t n)
{
    /* We need primes[0 .. n] so that primes[n] is prime(n+1). */
    size_t need = (size_t)(n + 1);
    if (prime_cnt > need)
        return;  /* already sufficient */

    /* Rough upper bound for the (n+1)-th prime. */
    idx_t new_limit = n * 20 + 10;
    if (new_limit < 100)
        new_limit = 100;

    if (new_limit <= last_limit)
        new_limit = last_limit * 2;

    if (primes != NULL)
        free(primes);  /* discard previous sieve */

    build_primes_upto(new_limit, &primes, &prime_cnt);
    last_limit = new_limit;

    /* If still not enough, keep doubling the limit. */
    while (prime_cnt <= need) {
        new_limit *= 2;
        free(primes);
        build_primes_upto(new_limit, &primes, &prime_cnt);
        last_limit = new_limit;
    }
}

/* Compute the n-th term of A129919 (n >= 1). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined for n >= 1 */

    /* Make sure primes array contains prime(n) and prime(n+1). */
    ensure_primes(n);

    /* Retrieve the required primes. */
    seq_t p_n     = (seq_t)primes[(size_t)n - 1];   /* prime(n)   */
    seq_t p_next  = (seq_t)primes[(size_t)n];       /* prime(n+1) */

    idx_t b = 1;
    for (;;) {
        seq_t candidate = (seq_t)b * p_next + p_n;
        if (is_prime_big(candidate))
            return candidate;
        ++b;
    }
}
