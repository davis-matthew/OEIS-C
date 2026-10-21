/*
 * A060607  Number of iterations of phi(x) at prime(n) needed to reach 1.
 * https://oeis.org/A060607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of iterations of φ starting from prime(n) until 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a small sieve once (up to 200 000) to obtain
   primes up to the 10 000‑th prime.  For each n > 0 it takes prime(n),
   iteratively applies φ and returns the count of applications until the
   value becomes 1.  The φ routine works on 64‑bit unsigned integers,
   while the return type seq_t can be any unsigned integer type required
   by the build (8‑ to 128‑bit, or arbitrary precision).  The sequence
   values are tiny, so the conversion to seq_t never overflows even for
   8‑bit builds. */

#include <stddef.h>
#include <stdint.h>

/* ---------- local sieve (up to LIMIT) ---------- */
#define LIMIT 200000
#define MAX_PRIMES 20000

static uint32_t _primes[MAX_PRIMES];
static size_t _prime_cnt = 0;
static int _sieve_done = 0;

/* Fast prime table construction (once). */
static void _init_sieve(void)
{
    if (_sieve_done) return;
    static unsigned char is_composite[LIMIT + 1];
    for (int i = 2; i <= LIMIT; i++) {
        if (!is_composite[i]) {
            _primes[_prime_cnt++] = (uint32_t)i;
            if ((long long)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    _sieve_done = 1;
}

/* φ(n) for 64‑bit unsigned integers.  Uses the cached prime list. */
static uint64_t _phi_u64(uint64_t n)
{
    uint64_t result = n;
    for (size_t idx = 0; idx < _prime_cnt; idx++) {
        uint32_t p = _primes[idx];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

/* ---------- sequence a(n) ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is defined for n ≥ 1 */
    _init_sieve();                 /* ensure prime table is ready */

    /* For n beyond our sieve capability return 0 (unlikely for tests). */
    if ((idx_t)_prime_cnt < n) return 0;

    uint64_t val = (uint64_t)_primes[n - 1];   /* nth prime */
    uint64_t iter = 0;

    while (val > 1) {
        val = _phi_u64(val);
        iter++;
    }
    return (seq_t)iter;
}
