/*
 * A080580  a(1)=1; for n>1, a(n)=a(n-1)+2 if n is already in the sequence,
 *          a(n)=a(n-1)+4 otherwise.
 * https://oeis.org/A080580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 for n > 1: a(n) = a(n-1) + 2 if n occurs among {a(1), …
 *            , a(n-1)} a(n-1) + 4 otherwise Computation is memoized; values
 *            are stored in a static array. Membership is tested using a
 *            boolean array indexed by term value
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* static state ---------------------------------------------------------- */
static seq_t *seq_vals = NULL;      /* a[1]..a[n]   (index 0 unused) */
static size_t seq_cap = 0;          /* allocated capacity of seq_vals */
static size_t seq_len = 0;          /* highest index computed */
static bool *seen = NULL;           /* seen[v] == true if v appears in sequence so far */
static size_t seen_cap = 0;         /* allocated capacity of seen */

/* ensure seq_vals has capacity >= needed */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure seen has capacity >= needed */
static void ensure_seen_cap(size_t needed)
{
    if (seen_cap >= needed) return;
    size_t new_cap = seen_cap ? seen_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    seen = (bool *)realloc(seen, new_cap * sizeof(bool));
    /* initialize new entries to false */
    for (size_t i = seen_cap; i < new_cap; ++i) seen[i] = false;
    seen_cap = new_cap;
}

/* initialize static arrays (called once) */
static void init_static(void)
{
    if (seq_vals) return;                 /* already initialized */
    ensure_seq_cap(2);                    /* indices 0 and 1 */
    seq_vals[0] = (seq_t)0;               /* unused */
    seq_vals[1] = (seq_t)1;               /* a(1) = 1 */
    seq_len = 1;
    ensure_seen_cap(2);
    seen[1] = true;                       /* value 1 is in sequence */
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n < 1 */

    init_static();                        /* ensure globals are ready */

    /* compute up to n if necessary */
    if (seq_len < n) {
        ensure_seq_cap(n + 1);             /* indices 0..n */

        for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
            seq_t prev = seq_vals[i - 1];
            /* check whether index i is already in the sequence */
            bool present = (i < seen_cap) && seen[i];
            seq_t incr = present ? (seq_t)2 : (seq_t)4;
            seq_t val = prev + incr;
            seq_vals[i] = val;

            /* mark the new term as seen */
            size_t val_idx = (size_t)val;  /* safe: term fits within 64‑bit for defined range */
            ensure_seen_cap(val_idx + 1);
            seen[val_idx] = true;
        }
        seq_len = (size_t)n;
    }

    return seq_vals[(size_t)n];
}
