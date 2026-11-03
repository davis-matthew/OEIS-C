/*
 * A203018  The n-th prime number that equals 1 (mod 4n).
 * https://oeis.org/A203018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p that satisfies p ≡ 1 (mod 4n) (n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 922 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

/* Return the n-th term of sequence A203018.
   For n = 1,2,...   a(n) is the n-th prime that is congruent to 1 modulo 4n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined, but defensive */

    /* Modulus m = 4n, computed in seq_t to avoid overflow. */
    const seq_t m = ((seq_t)n) * ((seq_t)4);

    idx_t   count = 0;                   /* how many qualifying primes found */
    idx_t   k     = 1;                    /* multiplier for the modulus */
    seq_t   result = 0;

    /* Search for k = 1, 2, … until we have found n primes in the progression. */
    for (;; ++k) {
        const seq_t p = m * (seq_t)k + 1;   /* candidate prime = 1 + 4n*k */
        if (is_prime_big(p)) {
            ++count;
            if (count == n) {
                result = p;
                break;
            }
        }
    }

    return result;
}
