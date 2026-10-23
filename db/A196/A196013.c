/*
 * A196013  Number of n X 4 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors within one of itself.
 * https://oeis.org/A196013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+2*a(n-2)+4*a(n-3)-a(n-4)-4*a(n-5)-2*a(n-6)-3*a(n-7)-3*a(n-8)+a(n-10)
 *            Initial values:
 *            a(1)=1,a(2)=1,a(3)=7,a(4)=25,a(5)=27,a(6)=91,a(7)=231,a(8)=455,a(9)=1135,
 *            a(10)=2693,a(11)=6041,a(12)=14257,a(13)=33349,a(14)=76941
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values: indices 0..memo_size */
static seq_t *memo = NULL;
static idx_t memo_size = 0;

/* Ensure the cache can hold terms up to n.  Reallocate if necessary. */
static void ensure_size(idx_t n)
{
    if (n <= memo_size) return;

    /* Allocate new array */
    seq_t *new_mem = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_mem) abort(); /* Allocation failure is fatal */

    /* Copy existing data */
    for (idx_t i = 0; i <= memo_size; ++i)
        new_mem[i] = memo ? memo[i] : 0;

    /* Zero the remaining entries */
    for (idx_t i = memo_size + 1; i <= n; ++i)
        new_mem[i] = 0;

    /* Replace old array */
    free(memo);
    memo = new_mem;
    memo_size = n;
}

/* Base values up to a(14) */
static const seq_t init[15] = {
    0,            /* unused index 0 */
    1,            /* a(1) */
    1,            /* a(2) */
    7,            /* a(3) */
    25,           /* a(4) */
    27,           /* a(5) */
    91,           /* a(6) */
    231,          /* a(7) */
    455,          /* a(8) */
    1135,         /* a(9) */
    2693,         /* a(10) */
    6041,         /* a(11) */
    14257,        /* a(12) */
    33349,        /* a(13) */
    76941         /* a(14) */
};

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 14) return init[(size_t)n];

    /* Prepare cache for n and for base values */
    ensure_size(n);
    if (memo_size < 14) ensure_size(14);
    for (idx_t i = 1; i <= 14; ++i)
        if (memo[i] == 0) memo[i] = init[(size_t)i];

    /* Compute missing terms up to n */
    for (idx_t i = 15; i <= n; ++i)
        if (memo[i] == 0) {
            seq_t v = memo[i-1];
            v += (seq_t)2 * memo[i-2];
            v += (seq_t)4 * memo[i-3];
            v += (seq_t)0 * memo[i-4]; /* placeholder to ease alignment */
            v -= memo[i-4];
            v -= (seq_t)4 * memo[i-5];
            v -= (seq_t)2 * memo[i-6];
            v -= (seq_t)3 * memo[i-7];
            v -= (seq_t)3 * memo[i-8];
            v += memo[i-10];
            memo[i] = v;
        }

    return memo[n];
}
