/*
 * A237168  Number of ways to write 2*n - 1 = 2*p + q with p, q, phi(p+1) - 1
 *          and phi(p+1) + 1 all prime, where phi(.) is Euler's totient
 *          function.
 * https://oeis.org/A237168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 555, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237168: a(n) = number of ways to write 2*n-1 = 2*p + q
   with p, q, φ(p+1)-1 and φ(p+1)+1 all prime.  */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define MAXV 20001      // covers 2*n-1 up to 19999 (for n<=10000)

/* sieve, prime list and totient values, built once */
static bool isprime[MAXV];
static unsigned primes[MAXV];
static size_t prime_cnt;
static uint32_t phi[MAXV];
static bool sieve_inited = false;

/* initialise sieve, primes and φ values */
static void init_sieve(void) {
    if (sieve_inited) return;
    for (int i = 0; i < MAXV; ++i) isprime[i] = true;
    isprime[0] = isprime[1] = false;
    for (int i = 2; i * i < MAXV; ++i) {
        if (isprime[i]) {
            for (int j = i * i; j < MAXV; j += i)
                isprime[j] = false;
        }
    }
    prime_cnt = 0;
    for (int i = 2; i < MAXV; ++i)
        if (isprime[i]) primes[prime_cnt++] = (unsigned)i;

    /* initialise totients */
    for (int i = 0; i < MAXV; ++i) phi[i] = (uint32_t)i;
    for (size_t k = 0; k < prime_cnt; ++k) {
        unsigned p = primes[k];
        for (int m = p; m < MAXV; m += p)
            phi[m] = (uint32_t)(phi[m] / p * (p - 1));
    }
    sieve_inited = true;
}

seq_t a(idx_t n) {
    init_sieve();
    if (n == 0) return (seq_t)0;          /* safety for idx_t<1 */
    seq_t target = (seq_t)((seq_t)2 * n - 1);   // 2*n-1
    idx_t t = (idx_t)target;                // fits into idx_t for n<=10000
    idx_t limit_p = t / 2;                  // p <= floor((t-2)/2)
    uint64_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && primes[i] <= limit_p; ++i) {
        idx_t p = primes[i];
        idx_t q = t - 2 * p;
        if (q < 2) continue;
        if (!isprime[q]) continue;
        idx_t m = p + 1;
        uint32_t phi_val = phi[m];
        idx_t phi_minus1 = (idx_t)phi_val - 1;
        idx_t phi_plus1  = (idx_t)phi_val + 1;
        if (phi_minus1 < 2 || phi_plus1 < 2) continue;
        if (!isprime[phi_minus1] || !isprime[phi_plus1]) continue;
        ++cnt;
    }
    return (seq_t)cnt;
}
