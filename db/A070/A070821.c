/*
 * A070821  Integer part of n/(lpf(n)*gpf(n)), where lpf = A020639 is the
 *          least prime factor and gpf = A006530 the greatest prime factor.
 * https://oeis.org/A070821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(n / (lpf(n) * gpf(n)))
 * where lpf(n) = least prime factor of n
 *       gpf(n) = greatest prime factor of n
 * a(1) = 1 (definition)
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Prime sieve cache ----------------------------------------------------- */
#define MAX_PRIME_SIEVE 1000000
#define MAX_PRIME_COUNT 85000
static uint32_t _prime_list[MAX_PRIME_COUNT];
static size_t _prime_cnt = 0;
static bool _primes_inited = false;

static void _init_primes(void) {
    if (_primes_inited) return;
    static uint8_t _sieve[MAX_PRIME_SIEVE + 1];
    memset(_sieve, 1, sizeof(_sieve));
    _sieve[0] = 0;
    _sieve[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= MAX_PRIME_SIEVE; ++i) {
        if (_sieve[i]) {
            for (uint32_t j = i * i; j <= (uint32_t)MAX_PRIME_SIEVE; j += i) {
                _sieve[j] = 0;
            }
        }
    }
    for (uint32_t i = 2; i <= (uint32_t)MAX_PRIME_SIEVE; ++i) {
        if (_sieve[i]) {
            _prime_list[_prime_cnt++] = i;
        }
    }
    _primes_inited = true;
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;    /* a(1) = 1 */
    _init_primes();

    uint64_t tn = n;
    uint64_t lpf = 0;
    uint64_t gpf = 0;

    for (size_t idx = 0; idx < _prime_cnt && (uint64_t)_prime_list[idx] * _prime_list[idx] <= tn; ++idx) {
        uint32_t p = _prime_list[idx];
        if (tn % p == 0) {
            if (lpf == 0) lpf = p;   /* first divisor encountered is the least prime factor */
            /* remove all powers of p */
            while (tn % p == 0) tn /= p;
            gpf = p;                 /* last divisor encountered in loop is the greatest prime factor so far */
        }
    }
    if (tn > 1) {                       /* remaining part > 1 is a prime factor */
        if (lpf == 0) lpf = tn;
        gpf = tn > gpf ? tn : gpf;
    }

    /* Compute floor(n / (lpf * gpf)) */
    seq_t prod = (seq_t)lpf * (seq_t)gpf;
    seq_t res  = (seq_t)n / prod;
    return res;
}
