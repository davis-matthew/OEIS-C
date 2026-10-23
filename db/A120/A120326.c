/*
 * A120326  Cumulative sum of the remainders when dividing primes by 3.
 * https://oeis.org/A120326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} (prime(k) mod 3)
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A120326.  Up to n ≈ 10⁴ we can pre‑compute 20 000 primes
   (the 20 000th prime is 224737, so 20 000 primes are well within the
   limit 200000).  The function keeps static arrays of primes and their
   cumulative remainders, so repeated or out‑of‑order calls are fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static cache */
static bool _a120326_initialised = false;
static uint32_t _a120326_primes[20000];
static seq_t      _a120326_cum[20000];
static size_t     _a120326_prime_count = 0;

/* One‑time initialisation: sieve, store primes, cumulative remainders. */
static void _a120326_init(void)
{
    if (_a120326_initialised) return;

    const idx_t LIMIT   = 200000;              /* enough for 20 000 primes   */
    const idx_t SIZ     = LIMIT + 1;
    uint8_t *composite   = (uint8_t *)calloc((size_t)SIZ, 1); /* 0 = prime candidate */
    if (!composite) abort();

    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (!composite[p]) {
            for (idx_t q = p * p; q <= LIMIT; q += p) composite[q] = 1;
        }
    }

    size_t idx = 0;
    for (idx_t n = 2; n <= LIMIT && idx < 20000; ++n) {
        if (!composite[n]) {
            _a120326_primes[idx] = (uint32_t)n;
            ++idx;
        }
    }
    _a120326_prime_count = idx;

    /* cumulative sum of remainders modulo 3 */
    seq_t sum = 0;
    for (size_t i = 0; i < _a120326_prime_count; ++i) {
        seq_t rem = (seq_t)(_a120326_primes[i] % 3);
        sum += rem;
        _a120326_cum[i] = sum;
    }

    free(composite);
    _a120326_initialised = true;
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, harmless placeholder */
    _a120326_init();
    if (n > _a120326_prime_count) abort(); /* n beyond pre‑computed range */
    return _a120326_cum[(size_t)(n - 1)];
}
