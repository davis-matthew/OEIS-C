/*
 * A052016  Primes with digits in descending order that differ exactly by 1.
 * https://oeis.org/A052016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   primes whose decimal digits form a consecutive descending run
 *            (e.g., 43, 76543, …). The sequence is obtained by generating all
 *            numbers with such digit patterns, testing each for primality,
 *            and sorting the primes in ascending order
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   cmp_primes, is_prime_big, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/is_prime_big.h"
#include "../utils/cmp_primes.h"

static seq_t primes[54];               /* maximum possible candidates */
static size_t primes_count = 0;
static bool primes_prepared = false;

/* Builds the list of all primes that satisfy the digit condition. */
static void prepare_primes(void)
{
    if (primes_prepared)
        return;

    char buf[12];          /* enough for 10 digits + null */
    for (uint8_t start = 1; start <= 9; ++start) {
        for (uint8_t len = 1; len <= start + 1; ++len) {
            size_t pos = 0;
            for (uint8_t i = 0; i < len; ++i) {
                buf[pos++] = '0' + (start - i);
            }
            buf[pos] = '\0';
            seq_t num = seq_from_str(buf);
            if (is_prime_big(num)) {
                primes[primes_count++] = num;
            }
        }
    }

    /* Sort the primes in ascending order. */
    if (primes_count > 1) {
        qsort(primes, primes_count, sizeof(seq_t), cmp_primes);
    }

    primes_prepared = true;
}

seq_t a(idx_t n)
{
    prepare_primes();

    if (n == 0 || (size_t)n > primes_count)
        return (seq_t)0;

    return primes[(size_t)n - 1];
}
