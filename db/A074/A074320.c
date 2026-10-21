/*
 * A074320  a(n) = sum of smallest and largest prime factors of n, for n>1;
 *          a(1)=2.
 * https://oeis.org/A074320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest_prime_factor(n) + largest_prime_factor(n), with
 *            a(1)=2
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)2;

    uint64_t temp = (uint64_t)n;
    uint64_t smallest = 0;
    uint64_t largest   = 0;

    /* Handle factor 2 */
    if ((temp & 1) == 0) {
        smallest = largest = 2;
        while ((temp & 1) == 0)
            temp >>= 1;
    }

    /* Trial division by odd numbers */
    for (uint64_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            if (smallest == 0)
                smallest = i;
            largest = i;
            while (temp % i == 0)
                temp /= i;
        }
    }

    /* Remaining prime factor */
    if (temp > 1) {
        if (smallest == 0)
            smallest = temp;
        largest = temp;
    }

    /* For n a power of 2, smallest already set to 2 and largest stays 2 */
    return (seq_t)(smallest + largest);
}
