/*
 * A037128  a(n) = nextprime(3^n) - nextprime(2^n).
 * https://oeis.org/A037128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextprime(3^n) - nextprime(2^n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"
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

/* Helper that returns the smallest prime > v */
static seq_t nextprime_seq(seq_t v)
{
    /* If v fits in 64‑bit, use the fast C routine */
    seq_t max_u64 = (seq_t)(~(uint64_t)0);
    if (v <= max_u64) {
        return (seq_t)next_prime((uint64_t)v);
    }
    /* Otherwise scan with the arbitrary‑precision primality test */
    seq_t cand = v;
    ++cand;
    while (!is_prime_big(cand)) {
        ++cand;
    }
    return cand;
}

seq_t a(idx_t n)
{
    /* Compute 2^n and 3^n as seq_t */
    seq_t two   = pow2(n);
    seq_t three = pow3(n);

    /* Next primes after these powers */
    seq_t p2 = nextprime_seq(two);
    seq_t p3 = nextprime_seq(three);

    /* Return the difference */
    return p3 - p2;
}
