/*
 * A104319  Primes which are the reverse concatenation of four consecutive
 *          cubes.
 * https://oeis.org/A104319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   cube, digit_len, isprime_big, pow10, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/push_seq.h"
#include "../utils/cube.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/isprime_big.h"

/* Sequence: primes obtained by concatenating four consecutive cubes in reverse order. */
seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static idx_t cap = 0;
    static idx_t size = 0;
    static idx_t next_k = 1;

    while (size < n) {
        seq_t c0 = cube((seq_t)next_k);
        seq_t c1 = cube((seq_t)(next_k + 1));
        seq_t c2 = cube((seq_t)(next_k + 2));
        seq_t c3 = cube((seq_t)(next_k + 3));

        idx_t len0 = digit_len(c0);
        idx_t len1 = digit_len(c1);
        idx_t len2 = digit_len(c2);
        idx_t len3 = digit_len(c3);

        seq_t candidate = c3;
        candidate = candidate * pow10(len2) + c2;
        candidate = candidate * pow10(len1) + c1;
        candidate = candidate * pow10(len0) + c0;

        if (isprime_big(candidate)) {
            push_seq(&primes, &cap, &size, candidate);
        }
        ++next_k;
    }

    return primes[n - 1];
}
