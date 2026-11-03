/*
 * A077815  2^phi(n) mod n^2, where phi is Euler's totient function A000010.
 * https://oeis.org/A077815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^phi(n) mod n^2
 * Range:     u8 n <= 15, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 825 (2026-11-01)
 * Helpers:   powmod, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for OEIS, but safe fallback */

    seq_t mod = (seq_t)n * ((seq_t)n);
    seq_t phi_n = totient(n);          // phi(n) fits in uint64_t for n ≤ 10⁴
    seq_t result = powmod((seq_t)2, (uint64_t)phi_n, mod);
    return result;
}
