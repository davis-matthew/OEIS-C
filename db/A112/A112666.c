/*
 * A112666  Semiprimes n such that 3*n + 4 is a square.
 * https://oeis.org/A112666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th semiprime such that 3*n + 4 is a perfect square
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u64, is_square, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by linear search with caching. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_square.h"
#include "../utils/mem.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* static state for caching sequence values */
static seq_t *seq = NULL;        /* array of found terms */
static idx_t seq_cap = 0;        /* capacity of seq */
static idx_t seq_size = 0;       /* current number of stored terms */
static idx_t last_checked = 0;   /* last candidate examined */

/* Return the n-th term of the sequence (1-based index).  */
seq_t a(idx_t n)
{
    /* safety: n >= 1 */
    if (n == 0) return 0;

    /* generate terms until we have at least n of them */
    while (seq_size < n)
    {
        last_checked++;  /* next candidate */

        /* check semiprimality */
        if (!is_semiprime_uint64((uint64_t)last_checked))
            continue;

        /* compute 3*last_checked + 4 in seq_t type */
        seq_t val = (seq_t)3 * (seq_t)last_checked + 4;

        /* check if this value is a perfect square */
        if (is_square(val))
        {
            /* store the candidate as a term of the sequence */
            push_seq(&seq, &seq_cap, &seq_size, (seq_t)last_checked);
        }
    }

    /* seq is indexed from 0, but the sequence is 1-based */
    return seq[n - 1];
}
