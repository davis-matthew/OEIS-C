/*
 * A179484  Primes p such that p+-180 are squares.
 * https://oeis.org/A179484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(idx_t n)
 *   Computes the n‑th prime p such that p-180 and p+180 are perfect squares.
 *   The condition is equivalent to finding divisors u,v of 360=2·180 with
 *   identical parity, then a=(v-u)/2 and p=a²+180.  Only the resulting
 *   primes are stored for repeated queries.
 *
 *   The sequence is finite; for indices beyond the last term the function
 *   returns 0 (not defined).
 */
#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* The sequence has at most a handful of terms; store them in a static array. */
    enum { MAX_PRIMES = 10 };        /* safety margin */
    static seq_t primes[MAX_PRIMES];
    static size_t count = 0;

    if (count == 0) {                  /* first call: generate all terms */
        const idx_t D = 360;           /* 2*d with d = 180 */
        idx_t i, j;
        for (i = 1; i * i <= D; ++i) {
            if (D % i != 0)
                continue;
            j = D / i;                /* pair (i,j), i <= j */

            /* u = i, v = j must have the same parity */
            if (((i & 1u) ^ (j & 1u)) != 0u)
                continue;

            idx_t a_val = (j - i) / 2;
            seq_t p = (seq_t)a_val * (seq_t)a_val + (seq_t)180;

            if (is_prime_big(p) && count < MAX_PRIMES) {
                primes[count++] = p;
            }
        }

        /* Simple insertion sort for the very small list */
        for (size_t i = 1; i < count; ++i) {
            seq_t key = primes[i];
            size_t j = i;
            while (j > 0 && primes[j - 1] > key) {
                primes[j] = primes[j - 1];
                --j;
            }
            primes[j] = key;
        }
    }

    /* Return the requested term or 0 if out of bounds. */
    if (n == 0 || n > count)
        return (seq_t)0;
    return primes[n - 1];
}
