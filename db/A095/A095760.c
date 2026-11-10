/*
 * A095760  Number of A095747-primes in range ]2^(2n-1),2^2n].
 * https://oeis.org/A095760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p with 2^(2n-1) < p <= 2^(2n) that are
 *            A095747 primes (1 at both ends, every interior mirrored bit pair
 *            differs); computed by enumerating the 2^(n-1) candidate bit
 *            patterns and primality-testing each
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (sizeof(seq_t) <= 16 && (unsigned long long)(2 * n) > 8ULL * sizeof(seq_t)) return 0;
    unsigned len = (unsigned)(2 * n);
    uint64_t total = (uint64_t)1 << (n - 1);
    seq_t count = 0;
    for (uint64_t m = 0; m < total; m++) {
        seq_t v = 1;
        v = v | ((seq_t)1 << (len - 1));
        for (unsigned i = 1; i + 1 <= (unsigned)n; i++) {
            unsigned b = (unsigned)((m >> (i - 1)) & 1);
            if (b) v = v | ((seq_t)1 << i);
            else   v = v | ((seq_t)1 << (len - 1 - i));
        }
        if (is_prime_seq(v)) count++;
    }
    return count;
}
