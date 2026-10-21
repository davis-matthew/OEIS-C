/*
 * A072804  n-th prime prime(n) written in base (prime(n) (mod 4)).
 * https://oeis.org/A072804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = representation of the n‑th prime p(n) in base (p(n) mod
 *            4); if p(n) ≡ 1 (mod 4) use base 1 (unary: a string of p(n)
 *            ones), if p(n) ≡ 3 (mod 4) use base 3 (ternary digits), p(n)=2
 *            uses base 2 (binary). The output is written in decimal digits
 *            (0–9) with most‑significant digit first
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t; we assume their usual semantics. */

/* --------------------------------------------------------------
   Prime table (simple sieve up to 200 000 – sufficient for n ≤ 10 000).
   The arrays are static so they survive across calls to a().
   -------------------------------------------------------------- */
#define PRIME_LIMIT 200000

static bool _is_prime[PRIME_LIMIT + 1];
static uint32_t _primes[PRIME_LIMIT + 1];
static int _prime_count = 0;

static void _init_primes(void)
{
    /* initialise the sieve */
    for (int i = 0; i <= PRIME_LIMIT; ++i)
        _is_prime[i] = true;
    _is_prime[0] = _is_prime[1] = false;

    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (_is_prime[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                _is_prime[j] = false;

    /* collect primes into the array */
    _prime_count = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (_is_prime[i])
            _primes[_prime_count++] = (uint32_t)i;
}

/* --------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool _initialized = false;
    if (!_initialized) {
        _init_primes();
        _initialized = true;
    }

    /* n is 1‑based */
    if (n == 0 || n > (idx_t)_prime_count)
        return 0;            /* out of range – should not occur */

    uint32_t p = _primes[(int)(n - 1)];   /* nth prime */
    int    base = (p == 2) ? 2 : (int)(p % 4);   /* 1, 2 or 3 */

    seq_t res = 0;

    if (base == 1) {                     /* unary – repeat '1' */
        for (idx_t i = 0; i < p; ++i)
            res = res * 10 + 1;
    } else {                              /* base 2 or 3 representation */
        /* collect digits in reverse order */
        uint8_t   digits[32];
        idx_t     len = 0;
        uint32_t  tmp = p;
        do {
            digits[len++] = tmp % base;
            tmp /= base;
        } while (tmp);

        /* build the decimal number from most‑significant to least digit */
        for (idx_t i = len; i > 0; --i)
            res = res * 10 + digits[i - 1];
    }

    return res;
}
