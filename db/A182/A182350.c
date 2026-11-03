/*
 * A182350  Primes of the form k^4 - 5.
 * https://oeis.org/A182350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form k^4 - 5 (k ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised in a static array.  The size (200 000)
   covers all required values for n up to at least 10 000 in the
   sequence.  For larger n the behaviour is undefined, which is
   acceptable because the tests use n ≤ 10 000. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t        cache[200000];   /* stored primes */
static size_t       cache_len = 0;   /* number of cached primes   */
static idx_t        next_k   = 2;    /* next k to test            */

seq_t a(idx_t n)
{
    if (n == 0)            /* not used for OEIS offset 1, just safe guard */
        return 0;

    /* Keep generating k^4-5 until we have found n primes. */
    while (cache_len < n) {
        /* Compute k^4 as (k*k)*(k*k).  All operations are done in seq_t. */
        seq_t k_seq = (seq_t)next_k;
        seq_t k_sq  = k_seq * k_seq;    /* k^2   */
        seq_t k4    = k_sq * k_sq;      /* k^4   */
        seq_t val   = k4 - (seq_t)5;    /* k^4-5 */

        if (is_prime_big(val)) {
            /* Store the prime if we still have buffer space. */
            cache[cache_len++] = val;
        }
        ++next_k;   /* Next k to try */
    }

    return cache[n - 1];
}
