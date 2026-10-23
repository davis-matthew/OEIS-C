/*
 * A171528  Denominator of (n-th composite/n).
 * https://oeis.org/A171528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(n, A002808(n))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the denominator of the reduced fraction (A002808(n))/n,
   where A002808(n) is the n-th composite number (starting with 4). */

#include <stddef.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t n) -> bool */
#include "../utils/gcd.h"        /* gcd(seq_t a, seq_t b) -> seq_t */

static seq_t *composites = NULL; /* array of nth composites, 0‑based */
static size_t compcnt   = 0;     /* number of composites stored */
static size_t compsc    = 0;     /* current capacity of composites */
static idx_t next_candidate = 4; /* next integer to test for compositeness */

/* Helper to grow the composites array */
static void grow_composites(size_t needed) {
    if (needed > compsc) {
        size_t new_cap = compsc ? compsc * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failure – just abort further growth */
        composites = tmp;
        compsc = new_cap;
    }
}

/* Precompute composites until we have at least 'n' of them */
static void ensure_composites(idx_t n) {
    while ((idx_t)compcnt < n) {
        /* test next_candidate for primality */
        if (!is_prime(next_candidate)) {
            grow_composites(compcnt + 1);
            composites[compcnt++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n = 0, but guard anyway */
    ensure_composites(n);
    seq_t comp = composites[n - 1];        /* nth composite, 1‑based */
    seq_t g = gcd((seq_t)n, comp);         /* gcd of n and the composite */
    return ((seq_t)n) / g;                 /* denominator of reduced fraction */
}
