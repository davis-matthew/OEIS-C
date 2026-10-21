/*
 * A083418  Smallest positive integer m such that m+i^2=0 mod p_i (i-th prime)
 *          for 1<=i<=n.
 * https://oeis.org/A083418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest positive integer m such that m + i^2 ≡ 0 (mod
 *            p_i) for i = 1..n, where p_i is the i‑th prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* compute b^e (mod m) where 0 ≤ b,m < 2^64, e < 2^64. */
static uint64_t mod_pow_uint64(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (unsigned __int128)r * b % m;
        b = (unsigned __int128)b * b % m;
        e >>= 1;
    }
    return r;
}

/* primes[0] = 2, primes[1] = 3, ...  */
static int primes[200000];
static int prime_cnt = 0;
static int last_checked = 1;

/* Ensure that primes[0..n-1] are filled.  */
static void ensure_primes(idx_t n) {
    while ((size_t)prime_cnt < n) {
        ++last_checked;
        bool isPrime = true;
        for (int i = 0; i < prime_cnt; ++i) {
            int p = primes[i];
            if ((long long)p * p > last_checked) break;
            if (last_checked % p == 0) { isPrime = false; break; }
        }
        if (isPrime) primes[prime_cnt++] = last_checked;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    ensure_primes(n);
    seq_t m = 0;                     /* current solution modulo M */
    seq_t M = 1;                     /* current modulus */

    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = (uint32_t)primes[i - 1];          /* i-th prime   */
        uint64_t sq = (uint64_t)i * (uint64_t)i;       /* i^2          */
        uint64_t target_val = sq % p;
        target_val = (p - target_val) % p;             /* desired residue */
        seq_t target = (seq_t)target_val;

        seq_t m_mod_p = m % (seq_t)p;

        seq_t diff = target;
        diff -= m_mod_p;
        diff += (seq_t)p;
        diff %= (seq_t)p;                           /* diff in [0,p-1] */

        uint64_t M_mod_p = (uint64_t)(M % (seq_t)p);
        uint64_t inv_m = mod_pow_uint64(M_mod_p, (uint64_t)p - 2, (uint64_t)p);

        seq_t t = (diff * (seq_t)inv_m) % (seq_t)p;
        m = m + t * M;
        M = M * (seq_t)p;
    }
    return m;
}
