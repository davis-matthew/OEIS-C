/*
 * A142939  Primes congruent to 31 mod 64.
 * https://oeis.org/A142939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 207, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 31 (mod 64) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t *special = NULL;
static size_t special_cnt = 0;
static size_t special_cap = 0;
static uint64_t next_candidate = 31ULL;   /* first number of the form 64*k+31 */

static void ensure_capacity(size_t need)
{
    if (special_cap >= need)
        return;
    size_t new_cap = special_cap ? special_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(special, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    special = tmp;
    special_cap = new_cap;
}

static void ensure_special(idx_t n)
{
    while (special_cnt < (size_t)n)
    {
        if (is_prime_small((seq_t)next_candidate))
        {
            ensure_capacity(special_cnt + 1);
            special[special_cnt++] = (seq_t)next_candidate;
        }
        next_candidate += 64ULL;   /* move to the next 64k+31 candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for offset 0, guard */
    ensure_special(n);
    return special[n - 1];
}
