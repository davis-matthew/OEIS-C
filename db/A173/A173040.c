/*
 * A173040  Single primes having no representation of the form n-+d(n), where
 *          d(n) is the number of divisors of n.
 * https://oeis.org/A173040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Single primes that cannot be written as m ± d(m) for any integer
 *            m. * Implementation: Sieve up to a fixed bound, compute all
 *            divisor counts, * mark numbers of the form m±d(m), then output
 *            primes not marked
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Build the first NMAX such primes once and keep them statically. */
static const idx_t NMAX = 4000000;                     /* bound for m */
static seq_t *seq_vals = NULL;                         /* array of sequence values */
static size_t seq_count = 0;                           /* number of ready values */
static bool initialized = false;

/* Helper: extend sequence to cover at least n values (unlikely needed). */
static void init_sequence(void)
{
    const uint32_t N = (uint32_t)NMAX;
    /* divisor count and linear sieve data. */
    uint16_t *dcount = (uint16_t*)calloc(N + 1, sizeof(uint16_t));
    uint32_t *spf    = (uint32_t*)calloc(N + 1, sizeof(uint32_t));
    uint8_t  *exp    = (uint8_t *)calloc(N + 1, sizeof(uint8_t));
    if (!dcount || !spf || !exp) {
        /* allocation failed – abort */
        abort();
    }
    dcount[1] = 1;
    size_t prime_cap = N / 10;
    uint32_t *primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) abort();
    size_t prime_cnt = 0;
    uint16_t maxd = 0;

    for (uint32_t i = 2; i <= N; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            exp[i] = 1;
            dcount[i] = 2;
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt && primes[j] <= spf[i]; ++j) {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (ip > N) break;
            spf[(uint32_t)ip] = p;
            if (p == spf[i]) {
                exp[(uint32_t)ip] = exp[i] + 1;
                uint32_t prev_exp = exp[i];
                uint32_t d = dcount[i];
                dcount[(uint32_t)ip] = (d / (prev_exp + 1)) * (prev_exp + 2);
            } else {
                exp[(uint32_t)ip] = 1;
                dcount[(uint32_t)ip] = dcount[i] * 2;
            }
        }
        if (dcount[i] > maxd) maxd = dcount[i];
    }

    /* Mark all numbers of the form m ± d(m). */
    const uint32_t MARK_SZ = N + maxd + 1;
    uint8_t *marked = (uint8_t*)calloc(MARK_SZ, sizeof(uint8_t));
    if (!marked) abort();

    for (uint32_t m = 2; m <= N; ++m) {
        uint16_t d = dcount[m];
        uint32_t v1 = (m > d) ? m - d : 0;
        if (v1 < MARK_SZ) marked[v1] = 1;
        uint32_t v2 = m + d;
        if (v2 < MARK_SZ) marked[v2] = 1;
    }

    /* Sieve primes up to MARK_SZ. */
    uint8_t *isprime = (uint8_t*)calloc(MARK_SZ, sizeof(uint8_t));
    if (!isprime) abort();
    for (uint32_t i = 2; i < MARK_SZ; ++i) isprime[i] = 1;

    uint32_t sqrtM = 0;
    while ((uint64_t)(sqrtM + 1) * (sqrtM + 1) < MARK_SZ) ++sqrtM;
    for (uint32_t i = 2; i <= sqrtM; ++i) {
        if (isprime[i]) {
            for (uint32_t j = i * i; j < MARK_SZ; j += i) isprime[j] = 0;
        }
    }

    /* Collect primes that are not marked. */
    size_t cap = 1024;
    seq_vals = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!seq_vals) abort();
    seq_count = 0;

    for (uint32_t p = 2; p < MARK_SZ; ++p) {
        if (isprime[p] && !marked[p]) {
            if (seq_count == cap) {
                cap *= 2;
                seq_vals = (seq_t*)realloc(seq_vals, cap * sizeof(seq_t));
                if (!seq_vals) abort();
            }
            seq_vals[seq_count++] = (seq_t)p;
        }
    }
    /* Shrink to real size. */
    seq_vals = (seq_t*)realloc(seq_vals, seq_count * sizeof(seq_t));

    /* Free temporary arrays. */
    free(dcount);
    free(spf);
    free(exp);
    free(primes);
    free(marked);
    free(isprime);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    /* n is 1‑based; seq_vals is 0‑based. */
    return seq_vals[(size_t)(n - 1)];
}
