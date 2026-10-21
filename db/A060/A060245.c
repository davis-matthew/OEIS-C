/*
 * A060245  Primes p such that the sum of the first p composite numbers is
 *          itself prime.
 * https://oeis.org/A060245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 11, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060245: primes p such that the sum of the first p composite
 * numbers is itself prime.
 * The function incrementally generates composite numbers and records
 * primes that satisfy the condition. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"     /* is_prime(idx_t) */
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t) */

/* Return the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* cached list of found primes */
    static seq_t *solutions = NULL;
    static size_t sol_cap   = 0;
    static size_t sol_len   = 0;

    /* state for generating composites */
    static idx_t next_num = 4;           /* next integer to test */
    static idx_t comp_cnt = 0;           /* how many composites seen */
    static seq_t comp_sum = 0;           /* sum of those composites */

    /* generate terms until we have at least n of them */
    while (sol_len < n)
    {
        /* find the next composite number */
        while (1)
        {
            if (!is_prime(next_num))          /* composite */
            {
                ++comp_cnt;
                comp_sum += (seq_t)next_num;
                ++next_num;
                break;
            }
            ++next_num;        /* skip primes */
        }

        /* if the count of composites is prime and the sum is prime,
           record the count as the next term of the sequence */
        if (is_prime(comp_cnt) && is_prime_big(comp_sum))
        {
            if (sol_len == sol_cap)
            {
                size_t new_cap = sol_cap == 0 ? 16 : sol_cap * 2;
                seq_t *tmp = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed */
                solutions = tmp;
                sol_cap = new_cap;
            }
            solutions[sol_len++] = (seq_t)comp_cnt;
        }
    }

    return solutions[n - 1];
}
