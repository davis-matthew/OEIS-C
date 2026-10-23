/*
 * A143207  Numbers with distinct prime factors 2, 3, and 5.
 * https://oeis.org/A143207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30 * A051037(n) where A051037 is the sequence of 2^i 3^j
 *            5^k with i,j,k ≥ 0 (the classic “ugly” numbers)
 * Range:     u8 n <= 7, u16 n <= 111, u32 n <= 1164, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Array that will contain the ugly numbers 1,2,3,4,5,6,… */
static seq_t *h      = NULL;   /* pointer to the array            */
static size_t h_cap  = 0;      /* current capacity of h           */
static size_t h_sz   = 0;      /* number of elements stored in h  */
static size_t i2 = 0, i3 = 0, i5 = 0;   /* indices for multiples of 2,3,5 */

/* Initialise the array on first use. */
static void init_state(void)
{
    if (h != NULL)
        return;
    h_cap = 128;
    h = (seq_t *)malloc(h_cap * sizeof(seq_t));
    if (!h)
        abort();                 /* OOM – terminate */
    h[0] = (seq_t)1;             /* 2^0 3^0 5^0 */
    h_sz = 1;
    i2 = i3 = i5 = 0;
}

/* Ensure that h contains at least n ugly numbers (n ≥ 1). */
static void ensure_h(idx_t n)
{
    init_state();
    if ((size_t)n <= h_sz)
        return;

    /* Grow the array if needed. */
    if (h_cap < (size_t)n) {
        size_t new_cap = h_cap ? h_cap : 128;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(h, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        h = tmp;
        h_cap = new_cap;
    }

    /* Generate further ugly numbers until we have n of them. */
    while (h_sz < (size_t)n) {
        seq_t next2 = h[(size_t)i2] * (seq_t)2;
        seq_t next3 = h[(size_t)i3] * (seq_t)3;
        seq_t next5 = h[(size_t)i5] * (seq_t)5;
        seq_t next = next2;
        if (next3 < next)
            next = next3;
        if (next5 < next)
            next = next5;
        h[h_sz++] = next;
        if (next == next2) ++i2;
        if (next == next3) ++i3;
        if (next == next5) ++i5;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;          /* undefined for indices < 1 */

    ensure_h(n);
    seq_t val = h[(size_t)(n - 1)];  /* h is 0‑based, n starts at 1 */
    return val * (seq_t)30;          /* enforce exponents ≥ 1 */
}
