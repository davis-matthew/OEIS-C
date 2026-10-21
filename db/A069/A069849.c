/*
 * A069849  Number of distinct primes obtained by inserting an 8 at all
 *          possible places in n. This includes prefixing.
 * https://oeis.org/A069849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct primes obtained by inserting an 8 at
 *            all positions in the decimal representation of n (including
 *            prefixing and suffixing)
 * Range:     u8 n <= 14, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   cmp_seq_t, is_prime_big, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/seq_from_str.h"
#include "../utils/is_prime_big.h"
#include "../utils/cmp_seq_t.h"

/* Number of distinct primes obtained by inserting an 8
   into the decimal representation of n. */
seq_t a(idx_t n)
{
    /* convert n to decimal string */
    char s[32];
    int len = snprintf(s, sizeof(s), "%llu", (unsigned long long)n);
    if (len <= 0) return 0;          /* numeric error – should not happen */

    /* store primes found */
    seq_t primes[16];                /* enough for any reasonable number of digits */
    size_t prime_cnt = 0;

    for (int pos = 0; pos <= len; ++pos) {
        /* build the candidate string with an '8' inserted at position pos */
        char cand[32];
        memcpy(cand, s, pos);                /* copy prefix */ 
        cand[pos] = '8';
        memcpy(cand + pos + 1, s + pos, len - pos); /* copy suffix */
        cand[len + 1] = '\0';

        /* convert candidate to seq_t */
        seq_t num = seq_from_str(cand);

        /* test primality */
        if (!is_prime_big(num))
            continue;

        /* check if already counted */
        bool duplicate = false;
        for (size_t k = 0; k < prime_cnt; ++k) {
            if (cmp_seq_t(&num, &primes[k]) == 0) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            primes[prime_cnt++] = num;
        }
    }

    return (seq_t)prime_cnt;
}
