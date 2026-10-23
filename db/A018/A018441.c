/*
 * A018441  Divisors of 414.
 * https://oeis.org/A018441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth smallest divisor of 414
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t target = 414ULL;
    seq_t divs[64];
    size_t cnt = 0;

    uint64_t num = target;
    seq_t primes[10];
    unsigned exps[10];
    size_t pcnt = 0;

    /* factor 414 */
    for (uint64_t p = 2; p * p <= num; ++p) {
        if (num % p == 0) {
            unsigned e = 0;
            while ((num % p) == 0) {
                num /= p;
                ++e;
            }
            primes[pcnt] = (seq_t)p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (num > 1) {
        primes[pcnt] = (seq_t)num;
        exps[pcnt]  = 1;
        ++pcnt;
    }

    /* generate all divisors */
    divs[cnt++] = (seq_t)1;          /* starting divisor */

    for (size_t pi = 0; pi < pcnt; ++pi) {
        seq_t p = primes[pi];
        unsigned e = exps[pi];
        size_t old_cnt = cnt;
        seq_t p_power = (seq_t)1;
        for (unsigned k = 1; k <= e; ++k) {
            p_power *= p;
            for (size_t i = 0; i < old_cnt; ++i) {
                divs[cnt++] = divs[i] * p_power;
            }
        }
    }

    /* sort ascending */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (divs[j] < divs[i]) {
                seq_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
        }
    }

    /* return nth divisor or 0 if out of range */
    if (n >= (idx_t)1 && (size_t)(n - 1) < cnt)
        return divs[n - 1];
    else
        return (seq_t)0;
}
