/*
 * A075054  Smallest k such that (n+1)(n+2)...(n+k) is divisible by n!.
 * https://oeis.org/A075054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that (n+1)(n+2)...(n+k) is divisible by
 *            n!. The function uses prime factorisation and a sliding window
 *            of exponents to find the minimal k efficiently
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LIMIT 20000          // must be at least 2*max n expected
static int primes[2500];
static int prime_cnt;
static uint32_t spf[MAX_LIMIT + 1];
static int prime_idx[MAX_LIMIT + 1];

/* -------------- one‑time initialization of the sieve ----------------- */
static void init_sieve(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* initialise smallest prime factor array */
    for (int i = 0; i <= MAX_LIMIT; ++i) spf[i] = i;
    for (int i = 2; (int64_t)i * i <= MAX_LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (int j = i * i; j <= MAX_LIMIT; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    /* collect primes and map prime to its index */
    memset(prime_idx, -1, sizeof(prime_idx));
    prime_cnt = 0;
    for (int i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            primes[prime_cnt] = i;
            prime_idx[i] = prime_cnt;
            ++prime_cnt;
        }
    }
}

/* --------------------------- main routine ----------------------------- */
seq_t a(idx_t n)
{
    init_sieve();
    /* array of required exponents for each prime */
    static seq_t req[2500];
    static seq_t cur[2500];
    int rem = 0;      /* number of primes whose requirement > 0 */

    /* compute exponents in n! */
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if (p > (int)n) {
            req[i] = 0;
            cur[i] = 0;
            continue;
        }
        seq_t e = 0;
        idx_t power = p;
        while (power <= n) {
            e += n / power;
            power *= p;
        }
        req[i] = e;
        cur[i] = e;
        if (e > 0) ++rem;
    }

    /* special case: n! = 1 (n = 1) -> answer is 1 */
    if (rem == 0) return 1;

    idx_t k = 1;
    while (true) {
        idx_t t = n + k;
        /* factor t using spf */
        while (t > 1) {
            int p = spf[t];
            int idx = prime_idx[p];
            /* only update if this prime was needed */
            if (cur[idx] > 0) {
                if (--cur[idx] == 0) --rem;
            }
            t /= p;
            while (t % p == 0) {
                t /= p;
                if (cur[idx] > 0) {
                    if (--cur[idx] == 0) --rem;
                }
            }
        }
        if (rem == 0) return k;
        ++k;
    }
}
