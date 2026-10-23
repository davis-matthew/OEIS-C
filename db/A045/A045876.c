/*
 * A045876  Sum of different permutations of digits of n (leading 0's
 *          allowed).
 * https://oeis.org/A045876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = ((10^digits(n)-1)/9) * (sum_digits(n)*N(n)/digits(n))
 * Range:     u8 n <= 38, u16 n <= 1025, u32 n <= 9858, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   factorial, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where N(n) = factorial(digits(n)) / Π factorial(freq(d))  */
#include "../utils/factorial.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Count digits of n and sum of those digits */
    idx_t temp = n;
    unsigned int freq[10] = {0};
    seq_t sum_digits = 0;         /* will stay small, but store as seq_t */
    idx_t dcnt = 0;               /* number of digits */

    do {
        unsigned int d = (unsigned int)(temp % 10);
        freq[d]++;
        sum_digits += d;
        temp /= 10;
        dcnt++;
    } while (temp > 0);

    /* Compute factorial of the number of digits */
    seq_t fact_d = factorial(dcnt);

    /* Compute denominator = product of factorials of digit frequencies */
    seq_t denom = 1;
    for (int i = 0; i < 10; ++i) {
        if (freq[i] > 1) {
            denom *= factorial((idx_t)freq[i]);
        }
    }

    /* Compute N = fact_d / denom */
    seq_t N = fact_d / denom;

    /* I = (10^dcnt - 1) / 9 */
    seq_t pow10_d = pow10(dcnt);
    seq_t I = (pow10_d - 1) / 9;

    /* Compute result = sum_digits * N * I / dcnt */
    seq_t prod = sum_digits * N;
    prod *= I;
    seq_t result = prod / dcnt;

    return result;
}
