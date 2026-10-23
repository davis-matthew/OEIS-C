/*
 * A107224  Numbers having in their Zeckendorf representation more odd than
 *          even terms.
 * https://oeis.org/A107224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having in their Zeckendorf representation more odd than even terms.
   The routine produces a(n) on demand by scanning integers sequentially and
   checking the greedy Zeckendorf decomposition of each candidate. */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *fib = NULL;          /* Fibonacci numbers (fib[0] = 1, fib[1] = 2) */
static idx_t  fib_len = 0;         /* number of Fibonacci numbers stored */
static size_t fib_cap = 0;         /* allocated capacity for fib array */

static seq_t *seq = NULL;          /* cached terms of the sequence */
static idx_t  seq_len = 0;         /* number of terms cached */
static size_t seq_cap = 0;         /* allocated capacity for seq array */

static seq_t last_seen = 0;        /* largest integer already examined */

/* Extend the Fibonacci table so that fib[fib_len-1] > limit. */
static void
ensure_fibs(seq_t limit)
{
    if (fib == NULL) {
        fib_cap = 8;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib) abort();
        fib[0] = 1;          /* F2 */
        fib[1] = 2;          /* F3 */
        fib_len = 2;
    }

    while (fib_len >= 2 && fib[fib_len-1] <= limit) {
        seq_t next = fib[fib_len-1] + fib[fib_len-2];
        /* In 64‑bit builds the sequence never overflows for n ≤ 10000. */
        if (fib_len == fib_cap) {
            fib_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            if (!tmp) abort();
            fib = tmp;
        }
        fib[fib_len++] = next;
    }
}

/* Ensure the term cache is large enough to hold min_len terms. */
static void
ensure_seq_cap(idx_t min_len)
{
    if (seq == NULL) {
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq_len = 0;
    }
    if (seq_len >= min_len) return;

    while (seq_cap < (size_t)min_len) seq_cap = seq_cap == 0 ? 8 : seq_cap * 2;
    seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* safety for callers using 0‑based indexes */

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        seq_t cand = last_seen + 1;
        ensure_fibs(cand);

        /* Greedy Zeckendorf decomposition – count odd/even Fibonacci terms. */
        seq_t rem = cand;
        idx_t i = fib_len;  /* index after the last valid element */
        int odd  = 0;
        int even = 0;

        while (rem > 0 && i > 0) {
            --i;
            if (rem >= fib[i]) {
                if ((fib[i] & 1) != 0) odd++; else even++;
                rem -= fib[i];
                if (i > 0) --i;      /* skip the next Fibonacci number */
            }
        }

        if (odd > even) {
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = cand;
        }

        last_seen = cand;
    }

    return seq[n-1];
}
