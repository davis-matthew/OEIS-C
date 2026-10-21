/*
 * A076108  Least positive n-th power that is the sum of n consecutive
 *          integers, or 0 if no such n-th power exists.
 * https://oeis.org/A076108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A076108: Least positive n-th power that is the sum of n consecutive
   integers, or 0 if no such power exists.
   Formula:
        if n % 4 == 0        → 0
        else let B = product of distinct odd primes dividing n
        a(n) = B^n
   The function below implements this directly. */

#include <stdbool.h>

/* fast exponentiation: base^exp for unsigned exp. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if ((n & 3) == 0)    /* n divisible by 4 → no solution */
        return 0;

    idx_t temp = n;
    seq_t base = 1;

    /* remove all factors of 2 */
    while ((temp & 1) == 0)
        temp >>= 1;

    /* factor the remaining odd part */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            base *= (seq_t)p;          /* include each distinct odd prime once */
            while (temp % p == 0)
                temp /= p;              /* remove all powers of p */
        }
    }

    /* if a prime > sqrt remains */
    if (temp > 1)
        base *= (seq_t)temp;

    return pow_seq_t(base, n);
}
