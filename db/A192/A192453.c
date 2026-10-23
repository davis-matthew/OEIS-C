/*
 * A192453  Numbers k such that -1 is a 4th power mod k.
 * https://oeis.org/A192453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that is not divisible by 4 and
 *            all its odd prime factors are ≡ 1 (mod 8)
 * Range:     u8 n <= 18, u16 n <= 2946, u32 n <= 4380, u64 n <= 4380,
 *            u128 n <= 4380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_vals = NULL;
static idx_t seq_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset starts at 1, this value is unused */

    if (n > seq_len)
    {
        idx_t required = n;
        idx_t limit = 100;                /* starting bound for the sieve */
        unsigned char *allowed = NULL;
        unsigned char *is_prime = NULL;
        size_t count = 0;
        seq_t *new_vals = NULL;

        while (1)
        {
            size_t sz = (size_t)limit + 1;
            allowed = (unsigned char*)malloc(sz);
            is_prime = (unsigned char*)malloc(sz);
            if (!allowed || !is_prime) exit(1);

            /* initialise all numbers as possible */
            for (size_t i = 0; i < sz; ++i)
                allowed[i] = 1;
            for (size_t i = 0; i < sz; ++i)
                is_prime[i] = 1;
            if (sz > 0) is_prime[0] = 0;
            if (sz > 1) is_prime[1] = 0;

            /* forbid multiples of 4 */
            for (size_t i = 4; i < sz; i += 4)
                allowed[i] = 0;

            /* classic prime sieve */
            for (size_t i = 2; i * i < sz; ++i)
                if (is_prime[i])
                    for (size_t j = i * i; j < sz; j += i)
                        is_prime[j] = 0;

            /* remove multiples of forbidden odd primes */
            for (size_t p = 3; p < sz; p += 2)
                if (is_prime[p])
                {
                    if (p % 8 != 1)            /* not ≡1 (mod 8) */
                        for (size_t j = p; j < sz; j += p)
                            allowed[j] = 0;
                }

            /* count how many values survived */
            count = 0;
            for (size_t i = 1; i < sz; ++i)
                if (allowed[i]) ++count;

            if (count >= required) break;   /* enough terms obtained */

            free(allowed);
            free(is_prime);
            limit <<= 1;                    /* enlarge search interval */
        }

        /* store all allowed numbers up to 'limit' */
        new_vals = (seq_t*)malloc(count * sizeof(seq_t));
        size_t pos = 0;
        size_t sz = (size_t)limit + 1;
        for (size_t i = 1; i < sz && pos < count; ++i)
            if (allowed[i])
                new_vals[pos++] = (seq_t)i;

        free(allowed);
        free(is_prime);

        free(seq_vals);      /* replace old sequence */
        seq_vals = new_vals;
        seq_len  = count;
    }

    return seq_vals[n - 1];
}
