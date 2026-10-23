/*
 * A018477  Divisors of 477.
 * https://oeis.org/A018477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth smallest divisor of 477 (1‑based)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* static cache for the 6 divisors of 477 */
static seq_t divisors[6];
static size_t divisors_len = 0;

/* Compute all divisors of 477 and store them sorted */
static void init_divisors(void)
{
    if (divisors_len) return;                /* already initialized */

    /* 477 = 3² × 53 */
    uint64_t n = 477ULL;
    uint64_t primes[2];
    uint8_t   exps[2];
    size_t cnt = 0;

    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint8_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt]   = 1;
        ++cnt;
    }

    divisors[0] = (seq_t)1;
    divisors_len = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint8_t  e = exps[i];
        size_t old_len = divisors_len;
        seq_t pow = (seq_t)1;
        for (uint8_t k = 1; k <= e; ++k) {
            pow = pow * (seq_t)p;
            for (size_t j = 0; j < old_len; ++j) {
                divisors[divisors_len++] = divisors[j] * pow;
            }
        }
    }

    /* sort ascending with a simple insertion sort */
    for (size_t i = 1; i < divisors_len; ++i) {
        size_t j = i;
        while (j > 0 && divisors[j] < divisors[j-1]) {
            seq_t tmp = divisors[j];
            divisors[j] = divisors[j-1];
            divisors[j-1] = tmp;
            --j;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_divisors();
    return (size_t)n <= divisors_len ? divisors[n-1] : (seq_t)0;
}
