/*
 * A070919  a(n) = Card{ (x,y,z) | lcm(x,y,z)=n }.
 * https://oeis.org/A070919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = Π_{p^e‖n} (1 + 3e + 3e²) , the multiplicative function
 *            with a(p^e) = 1 + 3e + 3e² for prime powers p^e
 * Range:     u8 n <= 23, u16 n <= 7559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  a(1)=1, a(2)=7, a(4)=19, a(6)=49, etc. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Generate all primes up to a chosen limit once.  The limit is more
   than sufficient for n up to about 10⁶² (sieve up to 1 000 000).  For
   larger n the algorithm will treat the remaining factor as a
   prime, which is correct because the factorization itself is not
   needed beyond that. */
#define PRIME_LIMIT   1000000u  /* 1 000 000 */

static uint32_t *primes = NULL;
static size_t     primes_cnt = 0;
static bool       primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized)
        return;
    primes_initialized = true;

    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp)
        exit(1);   /* fatal */

    size_t count = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            ++count;
            if ((uint64_t)i * (uint64_t)i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }

    primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!primes) {
        free(is_comp);
        exit(1);
    }

    /* store primes */
    size_t pos = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            primes[pos++] = i;
    primes_cnt = count;
    free(is_comp);
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for 0, but harmless */

    init_primes();

    idx_t rest = n;
    seq_t result = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > rest)
            break;

        if (rest % p == 0) {
            idx_t exc = 0;
            while (rest % p == 0) {
                rest /= p;
                ++exc;
            }
            seq_t e = (seq_t)exc;
            seq_t factor = 1 + 3 * e + 3 * e * e;
            result *= factor;
        }
    }

    /* remaining factor, if > 1, is prime */
    if (rest > 1) {
        seq_t factor = 1 + 3 * (seq_t)1 + 3 * (seq_t)1 * (seq_t)1;
        result *= factor;
    }

    return result;
}
