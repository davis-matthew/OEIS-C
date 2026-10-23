/*
 * A037931  Trajectory of 30 under prime factor concatenation procedure.
 * https://oeis.org/A037931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   concat_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Trajectory of 30 under prime factor concatenation procedure.
   a(0) = 30.
   For n > 0 the terms are generated as follows:
   – Let cur be the current number (initially 30).
   – List the prime factors of cur, with multiplicity, in ascending order.
   – Emit those primes, one per term.
   – When all factors have been emitted, form the next cur by
     concatenating the decimal representations of the factors in that
     same order, and continue.  (The process stabilises at the prime 547.) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/concat_primes.h"

/* return number of decimal digits of a non‑zero seq_t value */
static int digit_len_seq(seq_t x)
{
    int d = 0;
    do {
        x /= 10;
        ++d;
    } while (x);
    return d;
}

/* factor a 64‑bit integer into primes with multiplicity.
   primes[ ] must hold at least 20 entries; cnt will contain the count. */
static void factor_uint64(uint64_t n, uint64_t *primes, int *cnt)
{
    *cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            do {
                primes[(*cnt)++] = p;
                temp /= p;
            } while (temp % p == 0);
        }
    }
    if (temp > 1)
        primes[(*cnt)++] = temp;
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)30;

    uint64_t cur = 30;
    idx_t idx = 1;                     /* index of next output term */

    uint64_t primes[20];
    int cnt;

    while (true) {
        factor_uint64(cur, primes, &cnt);

        for (int i = 0; i < cnt; ++i) {
            if (idx == n) return (seq_t)primes[i];
            ++idx;
        }

        seq_t next;
        concat_primes(primes, cnt, &next);
        cur = (uint64_t)next;          /* fits for this sequence */
    }
}
