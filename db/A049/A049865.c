/*
 * A049865  Number of iterations of unitary totient function (A047994)
 *          required to reach 1 from n.
 * https://oeis.org/A049865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of iterations of the unitary totient function *
 *            U(n) = ∏_{p^k‖n} (p^k – 1). a(1)=0; for n>1, a(n)=1+a(U(n))
 * Range:     u8 n <= 1030, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Helper: compute the unitary totient of a 64‑bit integer.
 * For n > 1  U(n) = ∏(p^k – 1).  The result is strictly < n. */
static uint64_t _unitary_totient_uint64(uint64_t n) {
    if (n <= 1) return 1;          /* U(1) is defined as 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);   /* factor n into primes and exponents */

    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t part_seq = pow_uint(primes[i], exps[i]) - 1;   /* p^k – 1 as seq_t */
        uint64_t part = (uint64_t)part_seq;                  /* fits in uint64_t */
        result *= part;
    }
    return result;
}

/* Main sequence function.  Count how many iterations of U(n) are needed
 * to reduce n to 1.  The count itself fits in seq_t. */
seq_t a(idx_t n) {
    if (n <= 1) return 0;      /* a(1)=0 by definition */

    uint64_t cur = (uint64_t)n;
    seq_t steps = 0;
    while (cur > 1) {
        cur = _unitary_totient_uint64(cur);
        ++steps;
    }
    return steps;
}
