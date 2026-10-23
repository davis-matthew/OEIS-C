/*
 * A118965  Number of missing residues in Fibonacci sequence mod n.
 * https://oeis.org/A118965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd_uint64, _lcm_uint64, _pisano_period, _pisano_period_prime,
 *            _pisano_period_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A118965: Number of missing residues in Fibonacci sequence mod n
   Formula: a(n) = n - (number of distinct residues in one Pisano period of n) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_gcd_uint64.h"
#include "../utils/_lcm_uint64.h"
#include "../utils/_pisano_period.h"
#include "../utils/_pisano_period_prime.h"
#include "../utils/_pisano_period_prime_power.h"

/* gcd and lcm for uint64_t, needed for lcm of periods */

/* Main sequence function */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;          /* cast to uint64_t for arithmetic */
    if (nn <= 1) return (seq_t)0;       /* a(1)=0 */

    uint64_t period = _pisano_period(nn);

    /* Allocate array for seen residues */
    uint8_t *seen = (uint8_t*)calloc((size_t)nn, 1);
    if (!seen) abort();                /* memory allocation failed */

    uint64_t a_f = 0;
    uint64_t b_f = 1 % nn;
    unsigned count = 0;

    for (uint64_t i = 0; i < period; ++i) {
        if (!seen[a_f]) {
            seen[a_f] = 1;
            ++count;
        }
        uint64_t next = (a_f + b_f) % nn;
        a_f = b_f;
        b_f = next;
    }

    free(seen);

    uint64_t missing = nn - count;
    return (seq_t)missing;
}
