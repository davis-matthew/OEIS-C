/*
 * A134802  Concatenation of first n partition numbers of positive integers.
 * https://oeis.org/A134802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenate of the partition numbers p(1),p(2),…,p(n)
 *            where p(i) is the number of partitions of integer i. The
 *            sequence starts at offset 1, so a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for partition numbers */
static seq_t *p_arr = NULL;          /* p_arr[0] = 1, p_arr[1..] = p(i) */
static size_t p_capacity = 0;        /* allocated size of p_arr */
static bool p_initialized = false;   /* true after p_arr[0] is set */
static idx_t p_last = 0;             /* highest index computed so far */

/* Grow the partition array so that indices up to nmax are available. */
static void ensure_p_capacity(idx_t nmax)
{
    if (p_initialized && p_capacity > (size_t)nmax)
        return;

    size_t new_cap = p_capacity ? p_capacity << 1 : 1;
    while (new_cap <= (size_t)nmax)   /* make new_cap > nmax */
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(p_arr, new_cap * sizeof(seq_t));
    p_arr = tmp;
    p_capacity = new_cap;

    if (!p_initialized) {
        p_arr[0] = 1;            /* p(0) = 1 */
        p_initialized = true;
    }
}

/* Compute partition numbers p(i) for i in (p_last, nmax] using
   Euler's pentagonal theorem.  (The implementation works for any
   seq_t size; intermediate values stay within seq_t range.) */
static void compute_partitions(idx_t nmax)
{
    for (idx_t i = p_last + 1; i <= nmax; ++i) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = p_arr[i - pent1];
            if (k & 1) pos += term; else neg += term;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = p_arr[i - pent2];
            if (k & 1) pos += term; else neg += term;
        }
        p_arr[i] = pos - neg;
    }
    p_last = nmax;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset 1; undefined below, but harmless fall‑back */

    ensure_p_capacity(n);
    if (p_last < n)
        compute_partitions(n);

    seq_t result = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t pk = p_arr[i];

        /* Find the smallest power of 10 that is strictly larger than pk.
           That power equals 10^(number of decimal digits of pk). */
        seq_t pow10 = 1;
        while (pow10 <= pk) {
            pow10 *= 10;
        }

        /* Concatenate by shifting result left by pow10 and adding pk. */
        result = result * pow10 + pk;
    }

    return result;
}
