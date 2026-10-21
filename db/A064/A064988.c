/*
 * A064988  Multiplicative with a(p^e) = prime(p)^e.
 * https://oeis.org/A064988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, changed
 * Formula:   a(n) = ∏_{p^e∥n} prime(p)^e * (where prime(k) is the k‑th prime,
 *            e.g. prime(2)=3, prime(3)=5) * * The function factors n, then
 *            for each prime factor p replaces it by * the p‑th prime and
 *            raises that prime to the same exponent. The * product of these
 *            terms is the required sequence value
 * Range:     u8 n <= 39, u16 n <= 1279, u32 n <= 8767, u64 n <= 8767,
 *            u128 n <= 8767, big any n
 * Helpers:   factor_u64, pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"       /* factor_u64 */
#include "../utils/pow_seq_t.h"       /* pow_seq_t */

/* -------- Prime table ------------------------------------------- */
/* This table holds the primes in order: prime_table[i] = prime(i+1).
 * We pre‑compute a sufficient number of primes once.  For n ≤ 10000
 * we need the 10000‑th prime (104729), so a limit of 200000 gives
 * us a comfortable margin. */
static uint32_t *prime_table = NULL;  /* array of primes, 0‑based */
static size_t   prime_cnt   = 0;      /* number of primes stored */
static bool     prime_init  = false;  /* flag indicating initialisation */

static void init_prime_table(void)
{
    if (prime_init) return;
    const uint32_t LIMIT = 200000;  /* guarantees > 104729 */

    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve) return;            /* should never happen in test harness */

    size_t cap = LIMIT / 10 + 1; /* rough capacity heuristic */
    prime_table = (uint32_t *)malloc(cap * sizeof(uint32_t));
    prime_cnt = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            if (prime_cnt == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_table, cap * sizeof(uint32_t));
                if (!tmp) return;
                prime_table = tmp;
            }
            prime_table[prime_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
    prime_init = true;
}

/* --------- Main sequence function ------------------------------- */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;  /* a(1) = 1 */

    init_prime_table();

    /* Factor n into primes and exponents.  The helper writes into
     * arrays of size 64 which is ample for n ≤ 10000. */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t e   = exp[i];
        if (p == 0) continue;               /* safety, should not happen */
        if (p > prime_cnt) return (seq_t)0;  /* out of bounds; should not happen */

        /* prime at index p (p is a prime number, i.e. the index we need) */
        seq_t prime_at_p = (seq_t)prime_table[p - 1];
        seq_t power      = pow_seq_t(prime_at_p, e);
        result *= power;
    }
    return result;
}
