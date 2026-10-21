/*
 * A232190  a(n) is the number of primes of the form 2^b + 2n +- 2^k +- 1 and
 *          2^(b+2) - 2^b - 2n +- 2^k +- 1, where b is the length of the
 *          binary representation of 2n, and 0<k<=b.
 * https://oeis.org/A232190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes of the form 2^b + 2n ± 2^k ± 1
 *            or 2^(b+2) − 2^b − 2n ± 2^k ± 1, where b is the length of the
 *            binary representation of 2n, 1 ≤ k ≤ b
 * Range:     u8 n <= 31, u16 n <= 5927, u32 n <= 8515, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, is_prime_big, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 we return 0. */
    if (n == 0) return 0;

    /* 2n and its binary length b. */
    seq_t two_n = (seq_t)n * 2;
    unsigned b = bitlen_seq_t(two_n);

    /* 2^b and 3·2^b are needed many times. */
    seq_t pow2_b  = pow2(b);          /* 2^b */
    seq_t pow3_b  = seq_mul((seq_t)3, pow2_b); /* 3·2^b */

    /* An array to store distinct prime candidates.  The maximum
       number of distinct candidates per n is far below 256, so a
       fixed size buffer is adequate. */
    seq_t primes[256];
    size_t uniq = 0;

    /* Enumerate k = 1 … b and all eight ± combinations. */
    for (idx_t k = 1; k <= (idx_t)b; ++k) {
        seq_t pow2_k = pow2(k);

        /* Pre-calculate the eight numbers for this k. */
        seq_t cands[8] = {
            pow2_b + two_n + pow2_k + 1,
            pow2_b + two_n + pow2_k - 1,
            pow2_b + two_n - pow2_k + 1,
            pow2_b + two_n - pow2_k - 1,
            pow3_b - two_n + pow2_k + 1,
            pow3_b - two_n + pow2_k - 1,
            pow3_b - two_n - pow2_k + 1,
            pow3_b - two_n - pow2_k - 1
        };

        for (int i = 0; i < 8; ++i) {
            seq_t cand = cands[i];
            /* Skip numbers < 2 – they cannot be prime. */
            if (cand < 2) continue;
            /* Check primality. */
            if (!is_prime_big(cand)) continue;

            /* Avoid counting the same prime multiple times. */
            bool seen = false;
            for (size_t j = 0; j < uniq; ++j) {
                if (primes[j] == cand) { seen = true; break; }
            }
            if (!seen) {
                primes[uniq++] = cand;
                /* The array is large enough for all expected values. */
            }
        }
    }

    /* Convert the count of distinct primes to seq_t. */
    return (seq_t)uniq;
}
