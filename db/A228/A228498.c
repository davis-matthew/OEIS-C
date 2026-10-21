/*
 * A228498  a(n) = sigma(n^2) + phi(n^2) - 2n^2.
 * https://oeis.org/A228498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n^2) + phi(n^2) - 2*n^2
 * Range:     u8 n <= 15, u16 n <= 209, u32 n <= 3546, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* primes up to 10000 (enough for n<=10000) are generated once */
static uint32_t primes[1300];
static size_t prime_count = 0;
static bool primes_initialized = false;

/* Lazy initialization of the prime list */
static void init_primes(void) {
    if (primes_initialized)
        return;
    bool comp[10001] = {0};
    for (uint32_t i = 2; i * i <= 10000; ++i) {
        if (!comp[i]) {
            for (uint32_t j = i * i; j <= 10000; j += i)
                comp[j] = true;
        }
    }
    for (uint32_t i = 2; i <= 10000; ++i) {
        if (!comp[i])
            primes[prime_count++] = i;
    }
    primes_initialized = true;
}

/* Computes a(n) = sigma(n^2) + phi(n^2) - 2*n^2  */
seq_t a(idx_t n) {
    if (n == 0)   /* undefined; return 0 for safety */
        return 0;

    init_primes();

    /* Preserve original n for final calculation */
    seq_t orig_n = (seq_t)n;
    uint64_t temp = (uint64_t)n;

    /* Up to about 6 prime factors for n<=10000, so static arrays of
       length 8 are sufficient. */
    uint32_t pf[8];
    uint32_t pe[8];
    int fcnt = 0;

    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            pf[fcnt] = p;
            pe[fcnt] = e;
            fcnt++;
        }
    }
    if (temp > 1) {  /* remaining prime factor */
        pf[fcnt] = (uint32_t)temp;
        pe[fcnt] = 1;
        fcnt++;
    }

    seq_t sigma = 1;
    seq_t phi   = 1;

    for (int i = 0; i < fcnt; ++i) {
        uint32_t p  = pf[i];
        uint32_t e  = pe[i];
        seq_t pe_pow = 1;
        for (uint32_t j = 0; j < e; ++j)
            pe_pow *= p;            /* p^e */

        seq_t pe2 = pe_pow * pe_pow;          /* p^(2e) */
        seq_t sigma_factor = (pe2 * p - 1) / (p - 1);   /* (p^(2e+1)-1)/(p-1) */
        seq_t phi_factor   = (pe2 / p) * (p - 1);       /* p^(2e-1)*(p-1) */

        sigma *= sigma_factor;
        phi   *= phi_factor;
    }

    seq_t n2 = orig_n * orig_n;
    return sigma + phi - 2 * n2;
}
