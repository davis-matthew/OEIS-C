/*
 * A143867  a(n) is the number of n-digit integers in which the first k digits
 *          are divisible by k-th prime. Leading zeros are not allowed.
 * https://oeis.org/A143867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A143867:
   a(n) = number of n‑digit integers such that the first k digits
   (for every 1 ≤ k ≤ n) form a number divisible by the k‑th prime.
   The procedure enumerates all admissible prefixes up to the point
   where no further extensions are possible (in practice n≤10). */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for counts, initialized once */
    static bool initialized = false;
    static seq_t counts[32] = {0};
    static idx_t max_depth = 0;

    if (!initialized) {
        /* List of first primes (more than needed). */
        const uint32_t primes[] = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
           31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
          73, 79, 83, 89, 97, 101, 103, 107, 109, 113
        };
        const idx_t prime_count = sizeof(primes) / sizeof(primes[0]);

        /* Temporary arrays for current and next prefixes.
           The number of prefixes never exceeds 256 in this sequence. */
        uint64_t curr[256];
        uint64_t next[256];
        idx_t curr_len = 0, next_len = 0;

        /* Enumerate prefixes for increasing length k. */
        for (idx_t k = 1; k < prime_count; ++k) {
            next_len = 0;

            if (k == 1) {
                /* First digit cannot be zero. */
                for (idx_t d = 1; d <= 9; ++d) {
                    uint64_t v = (uint64_t)d;
                    if (v % (uint64_t)primes[0] == 0)
                        next[next_len++] = v;
                }
            } else {
                for (idx_t i = 0; i < curr_len; ++i) {
                    uint64_t prefix = curr[i];
                    for (idx_t d = 0; d <= 9; ++d) {
                        uint64_t v = prefix * 10ULL + (uint64_t)d;
                        if (v % (uint64_t)primes[k - 1] == 0)
                            next[next_len++] = v;
                    }
                }
            }

            /* Store count for this length. */
            counts[k] = (seq_t)next_len;
            max_depth = k;

            if (next_len == 0)
                break;          /* no further extensions possible */

            /* Move newly found prefixes to current for next step. */
            for (idx_t i = 0; i < next_len; ++i)
                curr[i] = next[i];
            curr_len = next_len;
        }

        initialized = true;
    }

    /* Return the requested term or zero if out of range. */
    if (n >= 1 && n <= max_depth)
        return counts[n];
    return (seq_t)0;
}
