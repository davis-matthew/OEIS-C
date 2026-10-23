/*
 * A118956  Numbers that cannot be written as 2^k + p with p prime < 2^k.
 * https://oeis.org/A118956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers that cannot be written as 2^k + p with p prime <
 *            2^k
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: build a prime table up to a given limit */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static bool *prime_flags = NULL;
static size_t sieve_limit = 0;

static void init_sieve(size_t limit) {
    if (prime_flags) free(prime_flags);
    sieve_limit = limit;
    prime_flags = (bool*)malloc((sieve_limit + 1) * sizeof(bool));
    if (!prime_flags) return;                /* malloc failure – sequence unusable */
    memset(prime_flags, 1, sieve_limit + 1); /* set all to true */
    if (sieve_limit >= 0) prime_flags[0] = 0;
    if (sieve_limit >= 1) prime_flags[1] = 0;
    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (prime_flags[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i) {
                prime_flags[j] = 0;
            }
        }
    }
}

seq_t a(idx_t n) {
    /* initialise 2‑million sieve – enough for n up to ~1e4 */
    if (!prime_flags) init_sieve(2000000);

    idx_t count = 0;
    seq_t m = 1;          /* current integer to test */

    while (1) {
        /* find 2^k <= m < 2^(k+1) : greatest power of two <= m */
        idx_t power = 1;
        while ((power << 1) <= m) {
            power <<= 1;
        }

        bool covered = false;
        if (m > power) {                          /* m cannot be a power of two */
            idx_t p = (idx_t)(m - power);         /* candidate prime */
            if (p > sieve_limit) {                /* extend sieve if needed */
                size_t new_limit = p * 2 + 10;     /* safety margin */
                init_sieve(new_limit);
            }
            if (prime_flags[p]) {
                covered = true;
            }
        }

        if (!covered) {
            ++count;
            if (count == n) return m;
        }
        ++m;
    }
}
