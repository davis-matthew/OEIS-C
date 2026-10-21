/*
 * A238005  Number of partitions of n into distinct parts such that (greatest
 *          part) - (least part) = (number of parts).
 * https://oeis.org/A238005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8488, u16 n <= 8488, u32 n <= 8488, u64 n <= 8488,
 *            u128 n <= 8488, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238005: Number of partitions of n into distinct parts such that
   (greatest part) - (least part) = (number of parts).
   Formula: a(n) = b(n) - c(n)
   where b(n) = max{i | i(i+1)/2 ≤ n}  (inverse triangular number) and
   c(n) = number of odd divisors of n.
*/
#include <stdint.h>
#include <stdbool.h>

/* Helper: compute b(n) = max i such that i*(i+1)/2 <= n */
static idx_t triangular_inverse(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 0;
    idx_t high = 1;
    /* Increase high until it exceeds the required value */
    while (high < n && ((unsigned __int128)high * (high + 1) / 2 <= (unsigned __int128)n))
        high <<= 1;
    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Helper: count odd divisors of n (i.e., number of divisors of n after removing all factors of 2) */
static idx_t odd_divisor_count(idx_t n)
{
    if (n == 0) return 0;
    idx_t count = 1;
    /* Remove all factors of 2 */
    while ((n & 1ULL) == 0)
        n >>= 1;
    /* Factor the remaining odd part */
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            count *= (exp + 1);
        }
    }
    if (n > 1)      /* remaining prime factor */
        count *= 2;
    return count;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard anyway */
    idx_t b = triangular_inverse(n);
    idx_t c = odd_divisor_count(n);
    seq_t result = (seq_t)b;
    result -= (seq_t)c;
    return result;
}
