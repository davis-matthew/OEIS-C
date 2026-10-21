/*
 * A065344  a(n) = binomial(2*n,n) mod ((n+1)*(n+2)).
 * https://oeis.org/A065344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) mod ((n+1)*(n+2))
 * Range:     u8 n <= 24, u16 n <= 330, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow_mod128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_mod128.h"

/* ---------- prime sieve ---------- */
#define MAX_PRIME 20000
static int primes[MAX_PRIME + 1];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    static bool is_comp[MAX_PRIME + 1];
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (int)i;
            if (i * i <= MAX_PRIME) {
                for (size_t j = i * i; j <= MAX_PRIME; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1, but guard */

    init_primes();

    unsigned long long nn = (unsigned long long)n;
    unsigned long long two_n = 2ULL * nn;
    unsigned __int128 mod = (unsigned __int128)(nn + 1) * (nn + 2);

    unsigned __int128 result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        unsigned int p = (unsigned int)primes[i];
        if ((unsigned long long)p > two_n) break;

        unsigned long long exp = 0;
        unsigned long long cur = p;
        while (cur <= two_n) {
            exp += two_n / cur - 2ULL * (nn / cur);
            cur *= p;
        }

        if (exp > 0) {
            unsigned __int128 pw = pow_mod128((unsigned __int128)p, exp, mod);
            result = (result * pw) % mod;
        }
    }

    return (seq_t)result;
}
