/*
 * A166039  Sums of three consecutive nonprimes A141468.
 * https://oeis.org/A166039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) + b(n+1) + b(n+2) where b(k) is the k‑th nonprime
 *            (0,1,4,6,8,9,10,…)
 * Range:     u8 n <= 62, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *nonprimes = NULL;   /* array of generated nonprimes */
static size_t nonprime_cap = 0;   /* current capacity of the array */
static size_t nonprime_len = 0;   /* number of elements currently stored */
static uint64_t next_num = 0;     /* next integer to test for primality */

/* Grow the nonprime array until it holds at least 'target' elements. */
static void ensure_nonprimes(idx_t target)
{
    if ((idx_t)nonprime_len >= target) return;

    while ((idx_t)nonprime_len < target) {
        seq_t val = (seq_t)next_num;
        if (!is_prime_big(val)) {           /* val is a nonprime: 0,1,4,6,… */
            if (nonprime_len == nonprime_cap) {
                size_t new_cap = nonprime_cap ? nonprime_cap * 2 : 1024;
                seq_t *tmp = (seq_t *)realloc(nonprimes, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                nonprimes = tmp;
                nonprime_cap = new_cap;
            }
            nonprimes[nonprime_len++] = val;
        }
        next_num++;
    }
}

/* Return a(n) for n >= 1.  For n < 1 the function returns 0 (undefined). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* We need the (n), (n+1), and (n+2)‑th nonprimes. */
    ensure_nonprimes(n + 2);

    size_t i = (size_t)(n - 1);          /* 0‑based index for b(n) */
    seq_t sum = nonprimes[i] + nonprimes[i + 1] + nonprimes[i + 2];
    return sum;
}
