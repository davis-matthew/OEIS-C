/*
 * A140849  Primes of the form 210k + 43.
 * https://oeis.org/A140849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140849: primes congruent to 43 modulo 210 */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Cache of previously found primes. */
    static seq_t *primes = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_cap = 0;
    static idx_t next_k = 0;  /* 0‑based index into the candidate list */

    if (n == 0) return 0; /* offset guard */

    while (prime_cnt < n)
    {
        seq_t candidate = (seq_t)210 * (seq_t)next_k + (seq_t)43;
        if (is_prime((idx_t)candidate))
        {
            grow_seq(&primes, &prime_cap, prime_cnt + 1);
            primes[prime_cnt++] = candidate;
        }
        ++next_k;
    }

    return primes[n - 1];
}
