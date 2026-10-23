/*
 * A049645  Numbers k such that the square of the number of divisors of k
 *          divides the sum of the divisors of k.
 * https://oeis.org/A049645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that τ(k)^2 divides σ(k)
 * Range:     u8 n <= 4, u16 n <= 523, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ = number of divisors, σ = sum of divisors */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_seq.h"

static bool init_primes_done = false;
static uint32_t primes[60000];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (init_primes_done) return;
    const int LIM = 50000;
    bool is_comp[LIM + 1];
    for (int i = 0; i <= LIM; ++i) is_comp[i] = false;
    for (int i = 2; i <= LIM; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((long long)i * i <= LIM) {
                for (long long j = 1LL * i * i; j <= LIM; j += i) {
                    is_comp[(int)j] = true;
                }
            }
        }
    }
    init_primes_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */
    init_primes();
    seq_t kth = 0;
    size_t found = 0;
    for (uint64_t k = 1; ; ++k) {
        uint64_t temp = k;
        seq_t tau = 1;
        seq_t sigma = 1;
        for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
            if (temp % primes[i] == 0) {
                unsigned exp = 0;
                while (temp % primes[i] == 0) {
                    temp /= primes[i];
                    ++exp;
                }
                tau *= (seq_t)(exp + 1);
                seq_t pow_val = pow_seq((seq_t)primes[i], exp + 1);
                seq_t part = (pow_val - (seq_t)1) / ((seq_t)primes[i] - (seq_t)1);
                sigma *= part;
            }
        }
        if (temp > 1) {
            tau *= (seq_t)2;
            seq_t pow_val = pow_seq((seq_t)temp, 2);
            seq_t part = (pow_val - (seq_t)1) / ((seq_t)temp - (seq_t)1);
            sigma *= part;
        }
        seq_t tau_sq = tau * tau;
        if (sigma % tau_sq == 0) {
            ++found;
            if (found == (size_t)n) return (seq_t)k;
        }
    }
}
