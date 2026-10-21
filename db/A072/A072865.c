/*
 * A072865  Smallest k > 1 dividing tau(k*n) (cf. A000005).
 * https://oeis.org/A072865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9971, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest k > 1 such that k divides τ(k·n), where τ(m) is the divisor count of m. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Pre‑compute primes up to 50000 – enough for all factorizations required. */
static uint32_t primes[50000];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    const size_t N = 50000;
    bool is_composite[N];
    for (size_t i = 2; i < N; ++i) if (!is_composite[i]) {
        primes[prime_cnt++] = (uint32_t)i;
        if (i * i < N)
            for (size_t j = i * i; j < N; j += i)
                is_composite[j] = true;
    }
    primes_ready = true;
}

/* Compute τ(x) for an arbitrary 64‑bit integer x. */
static seq_t tau_uint64(uint64_t x)
{
    seq_t result = 1;
    uint64_t remaining = x;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= remaining; ++i) {
        uint32_t p = primes[i];
        if (remaining % p == 0) {
            seq_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (remaining > 1)        /* the remaining part is a prime > sqrt(x) */
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 – not used by the tests */
    init_primes();

    uint64_t n64 = (uint64_t)n;
    for (seq_t k = 2; ; ++k) {
        uint64_t prod = n64 * (uint64_t)k;
        seq_t tau = tau_uint64(prod);
        if (tau % k == 0) return k;
    }
}
