/*
 * A177024  Numbers k such that 2^(k-1) mod k = number of divisors of k.
 * https://oeis.org/A177024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that 2^(k-1) mod k = d(k), where
 *            d(k) is the number of divisors of k
 * Range:     u8 n <= 24, u16 n <= 2500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/mod_pow.h"

/* Dynamic array to store found k-values */
static seq_t *values = NULL;
static size_t valcnt = 0;   /* number of stored values */
static size_t valcap = 0;   /* allocated capacity */

/* Largest integer checked so far (next candidate will be checked+1) */
static uint64_t checked = 0;

/* Ensure the values array has at least capacity 'needed' */
static void ensure_capacity(size_t needed)
{
    if (valcap >= needed) return;
    size_t newcap = valcap ? valcap * 2 : 16;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(values, newcap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failed */
    values = tmp;
    valcap = newcap;
}

/* Find the nth term of A177024.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, return 0 */

    /* Ensure we have enough pre‑computed values. */
    while (valcnt < (size_t)n) {
        checked++;  /* next integer to test */
        /* number of divisors of 'checked' */
        seq_t d = divisor_count_u64((uint64_t)checked);
        /* 2^(checked-1) mod checked */
        seq_t mod = mod_pow((seq_t)2, checked - 1, (seq_t)checked);
        if (mod == d) {
            ensure_capacity(valcnt + 1);
            values[valcnt++] = (seq_t)checked;
        }
    }
    return values[n - 1];
}
