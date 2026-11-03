/*
 * A154408  Primes p such that (p^2 + 1)/10 is also prime.
 * https://oeis.org/A154408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 690, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 983 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154408: primes p such that (p^2 + 1)/10 is also prime.
   The function computes the n‑th term (1‑based) with memoization.
   It uses simple primality tests from the utility library. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 – undefined for n=0 */

    static seq_t *seq = NULL;              /* cached sequence values      */
    static idx_t  seq_size = 0, seq_cap = 0;
    static idx_t  next_candidate = 3;      /* first odd prime candidate   */
    static bool    initialized = false;    /* one–time array allocation   */

    if (!initialized) {
        seq_cap = 64;
        seq = mem_new(seq_t, seq_cap);
        seq_size = 0;
        initialized = true;
    }

    while (seq_size < n) {
        /* Find the next prime candidate. */
        while (!is_prime(next_candidate))
            next_candidate += 2;

        /* (p^2 + 1) must be divisible by 10 for the quotient to be an integer. */
        seq_t p = (seq_t)next_candidate;
        seq_t t = p * p + 1;              /* p^2 + 1  (overflow is harmless for seq_t) */
        if ((t % (seq_t)10) != 0) {
            next_candidate += 2;
            continue;                     /* not a valid candidate */
        }

        seq_t q = t / (seq_t)10;           /* (p^2 + 1) / 10             */
        if (is_prime_big(q)) {             /* q must also be prime       */
            if (seq_size == seq_cap) {     /* grow storage if needed     */
                seq_cap <<= 1;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_size++] = next_candidate;   /* store the term           */
        }

        next_candidate += 2;               /* advance to following odd  */
    }

    return seq[n - 1];
}
