/*
 * A073272  A000040(n+1) - A073271(n).
 * https://oeis.org/A073272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = A000040(n+1) - A073271(n)
 *        = p_{n+1} - floor(p_n * p_{n+2} / p_{n+1})
 *
 * where p_k denotes the k‑th prime.
 *
 * The function computes the required primes on demand and uses only
 * integer arithmetic.  It works for any seq_t/idx_t pair defined by
 * the build system and is efficient enough for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1).  We need primes up to position n+2. */
    size_t limit = _prime_limit_for_idx(n + 2);

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Defensive check: ensure we have enough primes. */
    if (prime_cnt < (size_t)(n + 2)) {
        free(primes);
        return (seq_t)0;          /* unlikely, but safe fallback */
    }

    uint64_t p_n    = primes[n - 1];
    uint64_t p_np1  = primes[n];
    uint64_t p_np2  = primes[n + 1];

    uint64_t prod    = p_n * p_np2;          /* fits in 64‑bit for n <= 10000 */
    uint64_t floor_v = prod / p_np1;         /* floor(p_n * p_{n+2} / p_{n+1}) */

    int64_t tmp = (int64_t)p_np1 - (int64_t)floor_v;   /* signed difference */
    seq_t diff = (seq_t)tmp;

    free(primes);
    return diff;
}
