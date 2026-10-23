/*
 * A146116  Bell numbers (A000110) read mod 26.
 * https://oeis.org/A146116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell(n) mod 26 Bell numbers are defined by B(0)=1 and
 *            B(n+1) = sum_{k=0}^{n} C(n,k) * B(k) The recurrence is evaluated
 *            modulo 26
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint8_t *bell_mod26      = NULL;   /* B(n) mod 26 for n = 0 .. max_cached          */
static size_t   bell_cap        = 0;     /* allocated size of bell_mod26                 */
static size_t   bell_max_cached = 0;     /* highest n computed (inclusive)               */

static uint8_t *binom_mod26     = NULL;   /* current row of Pascal's triangle mod 26      */
static size_t   binom_cap       = 0;     /* allocated size of binom_mod26               */
static size_t   binom_max_idx   = 0;     /* index of the last row stored (n)            */

/* Ensure that the Bell numbers up to n (inclusive) are computed. */
static void ensure_bell(idx_t n)
{
    /* Initial allocation for n = 0 */
    if (bell_cap == 0) {
        bell_cap = 1;
        bell_mod26 = (uint8_t *)malloc(bell_cap);
        bell_mod26[0] = 1;          /* B(0) = 1 */
    }
    /* Initial allocation for binomial coefficients (row 0) */
    if (binom_cap == 0) {
        binom_cap = 1;
        binom_mod26 = (uint8_t *)malloc(binom_cap);
        binom_mod26[0] = 1;          /* C(0,0) = 1 */
    }

    /* Expand arrays if needed */
    if ((size_t)n + 1 > bell_cap) {
        size_t new_cap = (size_t)n + 1;
        uint8_t *tmp = (uint8_t *)realloc(bell_mod26, new_cap);
        bell_mod26 = tmp;
        bell_cap = new_cap;
    }
    if ((size_t)n + 2 > binom_cap) {  /* binom row needs n+1 elements, plus a spare slot */
        size_t new_cap = (size_t)n + 2;
        uint8_t *tmp = (uint8_t *)realloc(binom_mod26, new_cap);
        binom_mod26 = tmp;
        binom_cap = new_cap;
    }

    /* Compute missing Bell numbers */
    while (bell_max_cached < (size_t)n) {
        /* Current n to use in recurrence: m = bell_max_cached (0-based) */
        size_t m = bell_max_cached;

        /* Compute B(m+1) using current binomial row C(m,k) */
        unsigned int sum = 0;
        for (size_t k = 0; k <= m; ++k) {
            sum += (unsigned int)binom_mod26[k] * (unsigned int)bell_mod26[k];
            if (sum >= 26U) sum %= 26U;
        }
        bell_mod26[m + 1] = (uint8_t)(sum % 26U);
        bell_max_cached = m + 1;

        /* Update binomial row to n = m+1:
           binom(n,k) = binom(n-1,k-1) + binom(n-1,k)  (mod 26)          */
        binom_mod26[binom_max_idx + 1] = 0;  /* new element C(n,n) initially 0 */
        size_t idx = binom_max_idx + 1;
        while (idx >= 1) {
            uint8_t val = binom_mod26[idx] + binom_mod26[idx - 1];
            if (val >= 26U) val -= 26U;
            binom_mod26[idx] = val;
            if (idx == 0) break; /* avoid underflow */
            --idx;
        }
        binom_max_idx = m + 1;
    }
}

/* Main function returned to the judge */
seq_t a(idx_t n)
{
    ensure_bell(n);
    return (seq_t)bell_mod26[n];
}
