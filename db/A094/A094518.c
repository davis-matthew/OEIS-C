/*
 * A094518  Number of pairs (x,y) of divisors of n with x<y such that also x+y
 *          is a divisor of n.
 * https://oeis.org/A094518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 803, u16 n <= 9858, u32 n <= 9858, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of pairs (x,y) of divisors of n such that x<y and x+y divides n */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    /* factorise n by trial division */
    uint64_t tmp = (uint64_t)n;
    uint64_t fac[32];
    int exp[32];
    size_t pcnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            fac[pcnt] = p;
            exp[pcnt] = 0;
            while (tmp % p == 0) {
                ++exp[pcnt];
                tmp /= p;
            }
            ++pcnt;
        }
    }
    if (tmp > 1) {
        fac[pcnt] = tmp;
        exp[pcnt] = 1;
        ++pcnt;
    }

    /* generate all divisors */
    seq_t divs[256];
    size_t dcnt = 1;
    divs[0] = (seq_t)1;

    for (size_t i = 0; i < pcnt; ++i) {
        size_t old = dcnt;
        seq_t pow = 1;
        for (int e = 1; e <= exp[i]; ++e) {
            pow *= (seq_t)fac[i];
            for (size_t j = 0; j < old; ++j) {
                divs[dcnt++] = divs[j] * pow;
            }
        }
    }

    /* count valid pairs */
    seq_t nn = (seq_t)n;
    seq_t cnt = 0;
    for (size_t i = 0; i + 1 < dcnt; ++i) {
        for (size_t j = i + 1; j < dcnt; ++j) {
            seq_t sum = divs[i] + divs[j];
            if (sum != 0 && nn % sum == 0) {
                ++cnt;
            }
        }
    }
    return cnt;
}
