/*
 * A066284  a(n) = A066135(4*n).
 * https://oeis.org/A066284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066135(4*n)
 * Range:     u8 n <= 35, u16 n <= 575, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Small prime sieve up to a fixed limit */
#define PRIME_LIMIT 100000
static uint32_t _primes[PRIME_LIMIT / 10];
static size_t _prime_cnt = 0;
static int _primes_inited = 0;

static void _init_primes(void)
{
    if (_primes_inited)
        return;
    _primes_inited = 1;
    /* Simple sieve */
    unsigned char is_comp[PRIME_LIMIT + 1] = {0};
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            _primes[_prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
}

/* Modular exponentiation (generic for seq_t) */
static inline seq_t _mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1u)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1u;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    _init_primes();

    idx_t k = n * 4;                /* exponent for sigma_k */
    seq_t x = 2;                    /* candidate x starts at 2 */

    /* Temporary storage for prime factors of the candidate */
    uint64_t p_factors[32];
    uint32_t exp_factors[32];
    size_t pf_cnt;

    /* Temporary storage for divisors */
    seq_t divisors[256];
    size_t div_cnt;

    /* Repeat until we find a candidate that satisfies σ_k(x) % x == 0 */
    for (; ; ++x) {
        /* 1. Factor x (as a 64‑bit integer, x is small) */
        uint64_t m = (uint64_t)x;
        pf_cnt = 0;
        for (size_t i = 0; i < _prime_cnt && (uint64_t)_primes[i] * _primes[i] <= m; ++i) {
            uint64_t p = _primes[i];
            if (m % p == 0) {
                uint32_t e = 0;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                }
                p_factors[pf_cnt] = p;
                exp_factors[pf_cnt] = e;
                ++pf_cnt;
            }
        }
        if (m > 1) {                /* remaining prime factor */
            p_factors[pf_cnt] = m;
            exp_factors[pf_cnt] = 1;
            ++pf_cnt;
        }

        /* 2. Enumerate all divisors of x */
        div_cnt = 1;
        divisors[0] = 1;
        for (size_t f = 0; f < pf_cnt; ++f) {
            uint64_t p_val = p_factors[f];
            uint32_t e = exp_factors[f];
            size_t cur_cnt = div_cnt;
            for (size_t d = 0; d < cur_cnt; ++d) {
                seq_t val = divisors[d];
                for (uint32_t ee = 1; ee <= e; ++ee) {
                    val *= (seq_t)p_val;
                    divisors[div_cnt++] = val;
                }
            }
        }

        /* 3. Compute sigma_k(x) modulo x */
        seq_t sum = 0;
        seq_t modx = x;
        for (size_t di = 0; di < div_cnt; ++di) {
            seq_t d = divisors[di];
            seq_t t = _mod_pow(d, k, modx);
            sum = (sum + t) % modx;
        }

        if (sum == 0)
            return x;
    }
}
