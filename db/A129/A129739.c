/*
 * A129739  Primes in Somos-4 sequence (A006720).
 * https://oeis.org/A129739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes in the Somos‑4 sequence. Recurrence: a(n)*a(n‑4) =
 *            a(n‑1)*a(n‑3) + a(n‑2)² with a(1) = a(2) = a(3) = a(4) = 1. The
 *            function returns the n‑th prime appearing in that sequence
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t primes[20000];
static size_t prime_cnt = 0;
static seq_t last[4];
static bool initialized = false;

seq_t a(idx_t n)
{
    /* n ≥ 1 by contract; guard against zero for safety */
    if (n == 0) return 0;

    if (!initialized) {
        last[0] = last[1] = last[2] = last[3] = (seq_t)1;
        prime_cnt = 0;
        initialized = true;
    }

    /* Generate terms until we have at least n primes collected. */
    while (prime_cnt < (size_t)n) {
        /* Compute next term of the Somos‑4 recurrence */
        seq_t num  = last[3] * last[1] + last[2] * last[2];
        seq_t next = num / last[0];

        /* Shift the four‑term window to keep a(n‑4)…a(n‑1) for the next step */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = next;

        /* Store the term if it is prime. */
        if (is_prime_big(next)) {
            if (prime_cnt < 20000) {
                primes[prime_cnt++] = next;
            }
        }
    }

    return primes[n - 1];
}
