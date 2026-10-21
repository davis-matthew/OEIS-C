/*
 * A062799  Inverse Möbius transform of the numbers of distinct prime factors
 *          (A001221).
 * https://oeis.org/A062799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} ω(d) = Sum_{p|n} τ(n/p)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ω(d) is the number of distinct prime factors of d
   and τ(m) is the number of divisors of m.
   Equivalently,
      a(n) = Σ_{p|n} exp_p * τ(n) / (exp_p + 1)   (exp_p = exponent of prime p in n).
   The function factors n and uses the above identity. */

#include <stdint.h>

static const int MAX_PRIME_FACTORS = 64;  /* sufficient for 64‑bit integers */

seq_t a(idx_t n) {
    if (n <= 1) return 0;  /* a(1) = 0, a(0) irrelevant */

    uint64_t tmp = n;
    int primes[MAX_PRIME_FACTORS];
    int exps [MAX_PRIME_FACTORS];
    int cnt = 0;

    /* factorise n */
    for (uint64_t p = 2; p*p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            primes[cnt] = (int)p;   /* p fits in int for our usage */
            exps [cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {              /* remaining prime factor */
        primes[cnt] = (int)tmp;
        exps [cnt] = 1;
        ++cnt;
    }

    /* compute τ(n) = product (exp_i + 1) */
    seq_t tau = 1;
    for (int i = 0; i < cnt; ++i) {
        tau *= (seq_t)(exps[i] + 1);
    }

    /* sum τ(n/p) = tau / (exp_i + 1) * exp_i for each prime factor */
    seq_t result = 0;
    for (int i = 0; i < cnt; ++i) {
        seq_t term = tau / (seq_t)(exps[i] + 1); /* exact division */
        term *= (seq_t)(exps[i]);
        result += term;
    }

    return result;
}
