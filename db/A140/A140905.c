/*
 * A140905  Number of 5 X 6 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 5,6,n can be permuted, see
 *          formula.
 * https://oeis.org/A140905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^{5} ∏_{j=1}^{6} (n + i + j - 1) / (i + j - 1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The denominator is the same for all n and equals
    1^1·2^2·3^3·4^4·5^5·6^5·7^4·8^3·9^2·10 = 2^25·3^12·5^6·7^4
    We cancel the denominator primes on the fly while multiplying the
    numerators, so no intermediate overflow occurs for any build that
    can represent the final result. */

#include <stddef.h>
#include <stdint.h>

/*  Precomputed list of primes up to 101 (sqrt(10010) ≈ 100)  */
static const uint16_t small_primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97
};
#define NUM_SMALL_PRIMES (sizeof(small_primes)/sizeof(small_primes[0]))

/*  Denominator prime exponents: 2^25, 3^12, 5^6, 7^4  */
#define DEN_PRIMES_COUNT 4
static const uint8_t denom_primes[DEN_PRIMES_COUNT] = {2,3,5,7};
static const uint8_t denom_exponents_init[DEN_PRIMES_COUNT] = {25,12,6,4};

/*  Helper to factor a small integer and cancel with the denominator.
    `rem` points to the remaining denominator exponents array.
    `result` is updated in place. */
static void factor_and_cancel(idx_t x, uint8_t rem[DEN_PRIMES_COUNT], seq_t *result) {
    idx_t temp = x;
    /* trial divide by small primes */
    for (size_t pi = 0; pi < NUM_SMALL_PRIMES; ++pi) {
        uint16_t p = small_primes[pi];
        if ((idx_t)p * (idx_t)p > temp) break;
        int exp = 0;
        while (temp % p == 0) {
            ++exp;
            temp /= p;
        }
        if (exp == 0) continue;
        /* cancel with denominator if possible */
        for (size_t di = 0; di < DEN_PRIMES_COUNT; ++di) {
            if (denom_primes[di] == p) {
                int cancel = (rem[di] < exp) ? rem[di] : exp;
                rem[di] -= cancel;
                exp -= cancel;
                break;
            }
        }
        while (exp--) *result *= p;
    }
    /* remaining factor if >1 is a prime > sqrt(original)  */
    if (temp > 1) {
        uint16_t p = (uint16_t)temp;  /* temp fits in 16 bits */
        for (size_t di = 0; di < DEN_PRIMES_COUNT; ++di) {
            if (denom_primes[di] == p) {
                if (rem[di] > 0) rem[di]--;
                else *result *= p;
                return;
            }
        }
        /* new prime not in denominator list, multiply it */
        *result *= temp;
    }
}

/*  Main function computing the sequence term  */
seq_t a(idx_t n) {
    /* copy denominator exponents for this run */
    uint8_t rem[DEN_PRIMES_COUNT];
    for (size_t i = 0; i < DEN_PRIMES_COUNT; ++i) rem[i] = denom_exponents_init[i];

    seq_t result = 1;

    /* iterate over all cells of the 5×6 matrix   */
    for (idx_t i = 1; i <= 5; ++i) {
        for (idx_t j = 1; j <= 6; ++j) {
            idx_t factor = n + i + j - 1;   /* numerator factor  */
            factor_and_cancel(factor, rem, &result);
        }
    }

    /* At the end all denominator exponents should be zero.
       If any remain, divide the result by the remaining prime powers. */
    for (size_t di = 0; di < DEN_PRIMES_COUNT; ++di) {
        uint8_t exp = rem[di];
        while (exp--) result /= denom_primes[di];
    }

    return result;
}
