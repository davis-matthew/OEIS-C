/*
 * A089725  Let f(n) = (2n+1), g(n) = (n-1)/2. Then a(n) = f(f(f(...(n)))),
 *          where one gets prime at every step. If f(n) is not a prime then
 *          a(n) = g(g(g...(n)))) where one gets a prime at every step. If
 *          f(n) and g(n) both are composite then a(n) = 0.
 * https://oeis.org/A089725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089725: compute by repeated application of f(n)=2n+1 while prime,
   otherwise apply g(n)=(n-1)/2.  If a prime is never reached, return 0. */
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

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;

    /* first try the f chain */
    seq_t curr = val * (seq_t)2 + (seq_t)1;
    if (is_prime_big(curr)) {
        seq_t last = curr;
        while (1) {
            curr = curr * (seq_t)2 + (seq_t)1;
            if (is_prime_big(curr)) {
                last = curr;
            } else {
                return last;
            }
        }
    }

    /* f chain failed – try the g chain.  g is defined only for odd n. */
    if ((val & (seq_t)1) == (seq_t)0) {
        return (seq_t)0;
    }
    curr = (val - (seq_t)1) / (seq_t)2;
    if (!is_prime_big(curr)) {
        return (seq_t)0;
    }

    seq_t last = curr;
    while (1) {
        /* next g value exists only if current is odd */
        if ((curr & (seq_t)1) == (seq_t)0) {
            break;
        }
        curr = (curr - (seq_t)1) / (seq_t)2;
        if (is_prime_big(curr)) {
            last = curr;
        } else {
            break;
        }
    }
    return last;
}
