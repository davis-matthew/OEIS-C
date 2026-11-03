/*
 * A029832  A discrete version of the Mangoldt function: if n is prime then
 *          ceiling(log(n)) else 0.
 * https://oeis.org/A029832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(log(n)) if n is prime, else 0 (natural logarithm)
 * Range:     u8 n <= 65539, u16 n <= 65539, u32 n <= 65539, u64 n <= 65539,
 *            u128 n <= 65539, big any n
 * Validated: standard, pass, 131 terms through n = 8722 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    if (!is_prime(n))
        return 0;

    double log_val = log((double)n);
    unsigned int ceil_log = (unsigned int)ceil(log_val);
    return (seq_t)ceil_log;
}
