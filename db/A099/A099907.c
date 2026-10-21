/*
 * A099907  a(n) = C(2n-1,n-1) mod n^3.
 * https://oeis.org/A099907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n-1,n-1) mod n³
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 1646, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fact_exp_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fact_exp_uint32.h"

/* modular exponentiation with 128‑bit intermediate */
static uint64_t mod_pow_uint64(uint64_t base, uint32_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) {
            res = (unsigned __int128)res * base % mod;
        }
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* pre‑compute primes up to 20000 (enough for n up to 10000) */
#define MAX_PRIME 20000
static bool primes_ready = false;
static unsigned short primes_array[MAX_PRIME + 1];
static int primes_cnt;

static void init_primes(void)
{
    if (primes_ready) return;
    static bool sieve[MAX_PRIME + 1];
    for (int i = 0; i <= MAX_PRIME; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i) {
                sieve[j] = false;
            }
        }
    }
    primes_cnt = 0;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            primes_array[primes_cnt++] = (unsigned short)i;
        }
    }
    primes_ready = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but safe guard */
    uint64_t mod = (uint64_t)n * n * n;   /* n³ fits in 64‑bit for n≤10000 */
    if (mod == 1) return 0;              /* any value % 1 == 0, matches n=1 */

    init_primes();

    uint64_t result = 1 % mod;
    uint32_t two_n_minus1 = (uint32_t)(2 * n - 1);

    for (int i = 0; i < primes_cnt && primes_array[i] <= two_n_minus1; ++i) {
        uint32_t p = (uint32_t)primes_array[i];
        uint32_t e = fact_exp_uint32(two_n_minus1, p)
                   - fact_exp_uint32((uint32_t)n, p)
                   - fact_exp_uint32((uint32_t)(n - 1), p);
        if (e == 0) continue;
        uint64_t powp = mod_pow_uint64(p, e, mod);
        result = (unsigned __int128)result * powp % mod;
    }
    return (seq_t)result;
}
