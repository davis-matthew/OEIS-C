/*
 * A126864  a(n) = gcd(n, Product_{p|n} (p-1)), where the product is over the
 *          distinct primes, p, that divide n.
 * https://oeis.org/A126864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, Product_{p|n} (p-1))
 * Range:     u8 n <= 5417, u16 n <= 9877, u32 n <= 9877, u64 n <= 9877,
 *            u128 n <= 9877, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        /* Undefined for n < 1; return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Factor n into distinct primes */
    uint64_t primes[20];     /* enough for any 64‑bit n */
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= ((seq_t)primes[i] - (seq_t)1);
    }

    return gcd((seq_t)n, prod);
}
