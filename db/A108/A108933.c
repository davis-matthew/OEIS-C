/*
 * A108933  Numbers n such that the absolute value of (n - sum of the
 *          factorials of its digits) is prime.
 * https://oeis.org/A108933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 988 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that |n - sum of factorials of its digits| is prime. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

/* factorials of the digits 0..9, fits in 64‑bit */
static const uint64_t fact[10] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL,
    120ULL, 720ULL, 5040ULL, 40320ULL, 362880ULL
};

seq_t a(idx_t n)
{
    /* cache of found terms */
    static seq_t *terms   = NULL;
    static idx_t terms_cap = 0;   /* capacity in seq_t elements      */
    static idx_t terms_size = 0;  /* number of terms stored          */
    static idx_t tested  = 0;     /* last integer examined, starting
                                    * at 0 so first candidate is 1      */

    /* generate terms until we have at least n of them */
    while (terms_size < n) {
        ++tested;                       /* next integer to test */
        idx_t m = tested;               /* candidate number */

        /* compute sum of factorials of the digits of m */
        uint64_t sum = 0;
        idx_t tmp = m;
        if (tmp == 0)
            sum = fact[0];
        while (tmp > 0) {
            int d = (int)(tmp % 10);  /* current digit 0..9   */
            sum += fact[d];
            tmp /= 10;
        }

        /* absolute difference between m and the digit‑factorial sum */
        uint64_t diff = (m >= sum) ? (m - sum) : (sum - m);

        /* check primality of the difference */
        if (diff >= 2 && is_prime_u64(diff))
            push_seq(&terms, &terms_cap, &terms_size, (seq_t)m);
    }

    return terms[n - 1];
}
