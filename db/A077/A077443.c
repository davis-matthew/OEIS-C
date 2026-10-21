/*
 * A077443  Numbers k such that (k^2 - 7)/2 is a square.
 * https://oeis.org/A077443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=5, a(3)=13, a(4)=27 * Recurrence: a(n) = 6*a(n-2) -
 *            a(n-4) for n >= 5 * This follows from the Pell equation x^2 -
 *            2*y^2 = 7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *table = NULL;    /* 1‑based storage of computed terms */
static size_t cap = 0;         /* allocated size of table */
static idx_t last_filled = 0;  /* highest index for which table[index] is defined */

/* Ensure that table can store at least n+1 entries (index 0 unused). */
static void ensure_cap(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cap)
        return;

    size_t newcap = cap ? cap << 1 : 64;
    while (needed > newcap)
        newcap <<= 1;

    seq_t *newtab = (seq_t *)realloc(table, newcap * sizeof(seq_t));
    if (!newtab)
        abort();   /* out of memory */

    /* Zero‑initialise newly allocated slots for safety. */
    for (size_t i = cap; i < newcap; ++i)
        newtab[i] = (seq_t)0;

    table = newtab;
    cap   = newcap;
}

/* Initialise the first four terms of the sequence. */
static void init_table(void)
{
    if (!table)
    {
        ensure_cap(4);
        table[1] = (seq_t)3;
        table[2] = (seq_t)5;
        table[3] = (seq_t)13;
        table[4] = (seq_t)27;
        last_filled = 4;
    }
}

/* Return the n-th term of OEIS A077443. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n < 1, but spec guarantees n>=1 */

    init_table();
    ensure_cap(n);

    /* Return a pre‑computed value if possible. */
    if (n <= last_filled)
        return table[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last_filled + 1; i <= n; ++i)
    {
        if (i <= 4)           /* already initialised */
            continue;

        /* a(i) = 6*a(i-2) - a(i-4) */
        seq_t val = table[i - 2] * (seq_t)6 - table[i - 4];
        table[i] = val;
    }
    last_filled = n;
    return table[n];
}
