/*
 * A141566  Primes and exponents in the prime number decomposition of n
 *          replaced by primes at these positions.
 * https://oeis.org/A141566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A141566: a(n) is multiplicative. For a prime power p^e
   with e = 1, a(p) = prime(p);
   with e >= 2, a(p^e) = prime(p)^prime(e).
   For general n = ∏ p_i^e_i, 
   a(n) = ∏ prime(p_i)^( e_i==1 ? 1 : prime(e_i) ).
   Here prime(k) is the k‑th prime (prime(1)=2).
   
   The function below implements this definition using the provided
   utilities: factor_u64, get_prime, and seq_pow. It works for any
   unsigned integer index n >= 1 and for all supported seq_t types.
*/
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"

static seq_t ipow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;    /* a(1) = 2 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t ans = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t base = (seq_t)get_prime((idx_t)p);
        idx_t exponent = (e == 1) ? (idx_t)1 : (idx_t)get_prime((idx_t)e);
        seq_t term = ipow(base, exponent);
        ans = ans * term;
    }

    return ans;
}
