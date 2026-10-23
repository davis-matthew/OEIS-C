/*
 * A105854  Primes of the form 20*k + 3.
 * https://oeis.org/A105854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 816, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A105854: primes of the form 20*k + 3 (offset 1)
 * Formula: a(n) = the n‑th prime p such that p ≡ 3 (mod 20)
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrime64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the caller. */
    idx_t count = 0;
    uint64_t p = 3ULL;          /* first candidate (20*0 + 3) */

    while (true) {
        if (isPrime64(p)) {
            ++count;
            if (count == n)
                return (seq_t)p;
        }

        /* advance to the next candidate 20*k + 3 */
        p += 20ULL;

        /* Simple overflow guard – if wrap around occurred, stop. */
        if (p < 23ULL)   /* 23 is the second candidate, so any smaller value indicates overflow */
            return 0;
    }
}
