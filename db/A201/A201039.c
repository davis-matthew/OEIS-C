/*
 * A201039  Primes of the form 10n^3-9.
 * https://oeis.org/A201039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 10*n^3-9
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the nth prime in this form,
   where n=1 gives the first prime (71).  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t  *prime_list  = NULL;
static idx_t   prime_cnt   = 0;
static idx_t   prime_cap   = 0;
static seq_t   next_candidate = 2;

static void
ensure_primes(idx_t needed)
{
    while (prime_cnt < needed)
    {
        seq_t val = 10 * next_candidate * next_candidate * next_candidate - 9;
        if (is_prime_seq(val))
        {
            if (prime_cnt >= prime_cap)
            {
                prime_cap = (prime_cap == 0) ? 8 : prime_cap * 2;
                seq_t *tmp = (seq_t *)realloc(prime_list, sizeof(seq_t) * prime_cap);
                if (!tmp) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = val;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; 0 is unused, but harmless */
    ensure_primes(n);
    return prime_list[n-1];
}
