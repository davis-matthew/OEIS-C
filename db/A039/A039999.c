/*
 * A039999  Number of permutations of digits of n which yield distinct primes.
 * https://oeis.org/A039999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: number of distinct primes obtained by permuting the decimal
   digits of n and discarding any leading zeros. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"

/* Recursively generate every distinct permutation of the multiset of
   digits.  `counts` holds the remaining number of each digit to place;
   `current` is the partial number being built.  Every complete number
   is tested for primality and, if prime, increments `cnt`. */
static void
gen_perm(int depth, int total_len, int counts[10], seq_t current, seq_t *cnt)
{
    if (depth == total_len) {
        if (is_prime_big(current))
            (*cnt)++;
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (counts[d] == 0) continue;
        --counts[d];
        gen_perm(depth + 1, total_len, counts,
                 current * (seq_t)10 + (seq_t)d, cnt);
        ++counts[d];
    }
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* Obtain the decimal digits of n. */
    uint8_t dig[20];      /* enough for all 64‑bit numbers */
    size_t len;
    get_digits(sn, dig, &len);

    /* Count occurrences of each digit. */
    int counts[10] = {0};
    for (size_t i = 0; i < len; ++i)
        ++counts[dig[i]];

    seq_t cnt = 0;
    gen_perm(0, (int)len, counts, (seq_t)0, &cnt);
    return cnt;
}
