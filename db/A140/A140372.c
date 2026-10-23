/*
 * A140372  Primes of the form 26k + 9.
 * https://oeis.org/A140372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 544, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140372: primes of the form 26k + 9.  This function
 * finds the n‑th prime that is congruent to 9 modulo 26.
 *
 * The OEIS offset is 1, so a(1) is the first such prime.
 *
 * Implementation notes:
 *   - We use the provided utility `build_primes_upto` to generate all
 *     primes ≤ limit into a temporary array.
 *   - The limit is increased progressively until enough primes of the
 *     desired congruence class have been found.
 *   - The function works for any seq_t that can hold the result; if the
 *     result does not fit into seq_t the cast truncates, but this is
 *     acceptable: the specification only requires correctness for
 *     values that fit in seq_t.
 *
 * Dependencies:
 *   #include <stdlib.h>
 *   #include "../utils/build_primes_upto.h"
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; 0 is unused */

    /* Rough density of primes ≃ 1/(φ(26)=12).  Start with a bound
       that should contain at least n such primes.  The factor 250
       is generous but keeps the bound modest for n up to 10 000. */
    idx_t limit = n * 250;
    if (limit < 100) limit = 100;

    seq_t result = 0;

    while (1) {
        uint64_t *primes = NULL;
        size_t cnt = 0;

        build_primes_upto(limit, &primes, &cnt);

        size_t found = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if ((primes[i] % 26) == 9) {
                ++found;
                if (found == n) {
                    result = (seq_t)primes[i];
                    free(primes);
                    return result;
                }
            }
        }

        free(primes);
        /* Not enough found yet – increase the search range. */
        limit *= 2;
    }
}
