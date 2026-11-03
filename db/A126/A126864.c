/*
 * A126864  a(n) = gcd(n, Product_{p|n} (p-1)), where the product is over the
 *          distinct primes, p, that divide n.
 * https://oeis.org/A126864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, Product_{p|n} (p-1))
 * Range:     u8 n <= 5417, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9877 (2026-11-01)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

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
