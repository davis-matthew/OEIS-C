/*
 * A050194  a(n)=a(n-1)+a(n-2)-d, where d=a(n/4) if 4 divides n, else d=0; 2
 *          initial terms.
 * https://oeis.org/A050194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - d * where d = a(n/4) if 4 divides n,
 *            else d = 0. * Initial values: a(1) = 1, a(2) = 1
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* 1‑based array that holds the sequence */
static seq_t *seq = NULL;

/* largest index for which we have memory */
static idx_t cap = 0;

/* largest index that has already been computed */
static idx_t computed = 0;

/* has the base values been inserted? */
static bool initialized = false;

/* enlarge the array to hold at least 'needed' indices (1‑based) */
static void ensure_cap(idx_t needed)
{
    if (needed <= cap)
        return;
    idx_t newcap = cap ? (cap << 1) : 16;
    while (newcap < needed)
        newcap <<= 1;

    /* realloc and cast the result of malloc (required for C++) */
    seq_t *tmp = (seq_t *)realloc(seq, (newcap + 1) * sizeof(seq_t)); /* +1 for 1‑based indexing */
    seq = tmp;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for offset 1 – harmless */

    if (!initialized) {
        ensure_cap(2);
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)1;
        computed = 2;
        initialized = true;
    }

    if (n > computed) {
        ensure_cap(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t d = 0;
            if ((i & 3U) == 0)   /* 4 divides i – use bit mask for speed */
                d = seq[i / 4];
            seq[i] = seq[i - 1] + seq[i - 2] - d;
        }
        computed = n;
    }

    return seq[n];
}
