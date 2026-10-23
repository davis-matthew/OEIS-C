/*
 * A144624  a(n) = A078823(n) - n.
 * https://oeis.org/A144624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A078823(n) - n A078823(n): sum of all distinct binary
 *            numbers represented by proper substrings of n's binary
 *            representation (including the whole string). a(n) = A078823(n) -
 *            n
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 4096      /* must be power of two */

static seq_t hash_table[HASH_SIZE];
static unsigned char hash_used[HASH_SIZE];

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return 0;           /* sum of substrings = 0, so a(0)=0 */

    /* Determine bit length of n */
    idx_t tmp = n;
    size_t L = 0;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* Reset hash table */
    memset(hash_used, 0, HASH_SIZE);

    seq_t sum = 0;

    /* Enumerate all substrings */
    for (size_t i = 0; i < L; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < L; ++j) {
            seq_t bit = (seq_t)((n >> (L - 1 - j)) & 1);
            val = (val << 1) | bit;

            /* Insert into hash table if not seen before */
            size_t idx = (size_t)val & (HASH_SIZE - 1);
            while (hash_used[idx]) {
                if (hash_table[idx] == val)
                    break;   /* already seen */
                idx = (idx + 1) & (HASH_SIZE - 1);
            }
            if (!hash_used[idx]) {
                hash_used[idx] = 1;
                hash_table[idx] = val;
                sum += val;
            }
        }
    }

    return sum - (seq_t)n;
}
