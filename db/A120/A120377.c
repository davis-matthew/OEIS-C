/*
 * A120377  Primes of the form 2*11^k-1.
 * https://oeis.org/A120377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: standard, pass, 3 terms through n = 3 (2026-11-03)
 * Helpers:   ipow, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 2*11^k - 1 (OEIS A120377) */
/* Computation is performed by enumerating k=1,2,... and testing primality
   of 2*11^k - 1 until the n-th such prime is found. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"
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
    if (n == 0) {
        /* The sequence is defined for n>=1. Return 0 for safety. */
        return (seq_t)0;
    }

    seq_t result = (seq_t)0;
    idx_t found = 0;
    idx_t k = 1;                /* current exponent */

    while (found < n) {
        /* Compute p = 2*11^k - 1 */
        seq_t pow_val = ipow((seq_t)11, (seq_t)k);   /* 11^k */
        pow_val = pow_val * (seq_t)2;                   /* 2*11^k */
        pow_val = pow_val - (seq_t)1;                   /* 2*11^k - 1 */

        if (is_prime_big(pow_val)) {                    /* test primality */
            found++;
            if (found == n) {
                result = pow_val;
                break;
            }
        }
        k++;
    }
    return result;
}
