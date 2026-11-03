/*
 * A140120  Primes of the form prime(x)*prime(x+1) - (prime(x+1)-prime(x)).
 * https://oeis.org/A140120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)*prime(n+1) - (prime(n+1)-prime(n)) Returns the
 *            n-th prime value of this expression (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
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
    /* use 64‑bit primes for the base calculations */
    uint64_t p_prev = 2;            /* first prime */
    uint64_t p_cur  = 3;            /* second prime */
    idx_t    found = 0;

    while (found < n) {
        seq_t prev = (seq_t)p_prev;
        seq_t cur  = (seq_t)p_cur;

        /* compute candidate */
        seq_t val = prev * cur - (cur - prev);

        /* keep only prime results */
        if (is_prime_big(val)) {
            ++found;
            if (found == n)
                return val;
        }

        /* advance to next pair of consecutive primes */
        p_prev = p_cur;
        p_cur  = next_prime(p_cur + 1);
    }

    /* unreachable - the loop returns once n is reached */
    return (seq_t)0;
}
