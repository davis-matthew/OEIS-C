/*
 * A175854  Number of anagrams of n that are divisible by exactly 3 primes
 *          (counted with multiplicity).
 * https://oeis.org/A175854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of anagrams of n that are divisible by exactly
 *            three primes (counted with multiplicity)
 * Range:     u8 n <= 1696, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

/* Recursive generation of distinct permutations of the digits of n.
   `freq[d]` holds the remaining count of digit d that can be used.  */
static void permute(idx_t pos, seq_t cur, uint8_t *freq, idx_t len,
                    seq_t *cnt)
{
    if (pos == len) {
        /* `cur` is the complete permutation.  Count it if Ω(cur) == 3. */
        if (bigomega((idx_t)cur) == 3)
            ++(*cnt);
        return;
    }

    for (int d = 0; d < 10; ++d) {
        if (!freq[d]) continue;
        if (pos == 0 && d == 0) continue;   /* leading zero is forbidden */
        freq[d]--;
        permute(pos + 1, cur * 10 + d, freq, len, cnt);
        freq[d]++;
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    /* Determine digit frequencies of n. */
    uint8_t freq[10] = {0};
    idx_t temp = n;
    idx_t len = 0;

    if (temp == 0) {
        freq[0] = 1;
        len = 1;
    } else {
        while (temp > 0) {
            freq[temp % 10] += 1;
            temp /= 10;
            ++len;
        }
    }

    seq_t result = 0;
    permute(0, (seq_t)0, freq, len, &result);
    return result;
}
