/*
 * A068546  Numbers k such that k*tau(k)>4*prime(k).
 * https://oeis.org/A068546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A068546: Numbers k such that k*tau(k) > 4*prime(k)
 *  a(n) = the nth integer k for which k*tau(k) > 4*prime(k).
 *  Computed by scanning integers k starting at 1, evaluating the
 *  inequality using divisor and prime functions.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../utils/get_bit.h"

/* towering constant – size sufficient for n up to 10 000 */
static const uint32_t MAX_K = 10000000;          /* integers to test            */
static const uint32_t PRIMES_NEEDED = MAX_K;     /* need first MAX_K primes     */

/* static storage for the sequence */
static seq_t *seq_arr    = NULL;   /* sequence values */
static size_t seq_len    = 0;      /* number of values stored */
static int    initialized = 0;

/* bit‑set helpers for the prime sieve */
static inline void set_bit(unsigned char *tab, uint32_t i) {
    tab[i >> 3] |= (1U << (i & 7));
}

/* initialise all tables and compute the sequence up to MAX_K */
static void init(void) {
    /* 1. divisor function tau(k) via SPF (smallest prime factor) */
    uint32_t *spf = (uint32_t*)calloc(MAX_K + 1, sizeof(uint32_t));
    uint16_t *tau = (uint16_t*)calloc(MAX_K + 1, sizeof(uint16_t));
    uint32_t *prime_sieve_spf = (uint32_t*)malloc((MAX_K / 10) * sizeof(uint32_t));
    uint32_t pcnt = 0, i, j;

    for (i = 2; i <= MAX_K; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            prime_sieve_spf[pcnt++] = i;
        }
        for (j = 0; j < pcnt && prime_sieve_spf[j] <= spf[i] &&
                 (uint64_t)i * prime_sieve_spf[j] <= MAX_K; ++j) {
            uint32_t x = i * prime_sieve_spf[j];
            spf[x] = prime_sieve_spf[j];
        }
    }

    /* compute tau using spf */
    tau[1] = 1;
    for (i = 2; i <= MAX_K; ++i) {
        uint32_t p = spf[i];
        uint32_t m = i / p;
        int e = 1;
        while (m % p == 0) {
            ++e;
            m /= p;
        }
        tau[i] = (uint16_t)(tau[m] * (e + 1));
    }

    free(spf);
    free(prime_sieve_spf);

    /* 2. generate the first PRIMES_NEEDED primes using a bitset sieve */
    double dk = (double)MAX_K;
    double bound = dk * (log(dk) + log(log(dk))) + 3.0;     /* Rosser upper bound */
    uint32_t lim = (uint32_t)ceil(bound);

    unsigned char *is_comp = (unsigned char*)calloc((lim >> 3) + 1, sizeof(unsigned char));
    /* 0 and 1 are composite */
    set_bit(is_comp, 0);
    set_bit(is_comp, 1);

    for (i = 2; (uint64_t)i * i <= lim; ++i) {
        if (!get_bit(is_comp, i)) {
            for (uint64_t j = (uint64_t)i * i; j <= lim; j += i)
                set_bit(is_comp, (uint32_t)j);
        }
    }

    uint32_t *primes = (uint32_t*)malloc(PRIMES_NEEDED * sizeof(uint32_t));
    uint32_t pidx = 0;
    for (i = 2; pidx < PRIMES_NEEDED && i <= lim; ++i) {
        if (!get_bit(is_comp, i))
            primes[pidx++] = i;
    }
    /* pidx should equal PRIMES_NEEDED; if not, the bound was insufficient */
    /* but sequence generation will not require more primes */

    free(is_comp);

    /* 3. generate the sequence up to MAX_K */
    seq_len = 0;
    size_t seq_cap = 20000;
    seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));

    for (i = 1; i <= MAX_K; ++i) {
        seq_t prod = (seq_t)i * (seq_t)tau[i];
        seq_t thr  = (seq_t)4 * (seq_t)primes[i - 1];
        if (prod > thr) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
            }
            seq_arr[seq_len++] = (seq_t)i;
        }
    }

    free(tau);
    free(primes);
    initialized = 1;
}

/* Public function required by the problem statement */
seq_t a(idx_t n) {
    if (!initialized) init();
    if (n == (idx_t)0 || n > seq_len) return 0;
    return seq_arr[n - 1];
}
