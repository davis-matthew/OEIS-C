/*
 * A131880  a(n) = the n-th positive integer which is coprime to (2^n -1).
 *          a(n) = the n-th term of row n in triangle A131879.
 * https://oeis.org/A131880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer coprime to 2^n - 1
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Handle trivial case */
    if (n == 1) {
        return (seq_t)1;
    }
    /* Shortcut for prime n: a(n) = n */
    if (is_prime(n)) {
        return (seq_t)n;
    }

    idx_t count = 0;
    /* Enumerate positive integers and count those coprime to 2^n-1 */
    for (idx_t k = 1; ; ++k) {
        if (k == 1) {
            /* 1 is coprime to every integer */
            ++count;
            if (count == n) {
                return (seq_t)1;
            }
            continue;
        }
        seq_t mod = (seq_t)k;
        /* Compute 2^n mod k */
        seq_t pow = mod_pow((seq_t)2, n, mod);
        /* val = (2^n - 1) mod k */
        seq_t val = pow + mod - 1;
        val %= mod;
        if (val != 0 && gcd(mod, val) == (seq_t)1) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
