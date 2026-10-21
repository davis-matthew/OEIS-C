/*
 * A080574  a(1)=1, a(n)=a(n-1)+1 if n is odd and already in the sequence,
 *          a(n)=a(n-1)+2 if n is even and already in the sequence,
 *          a(n)=a(n-1)+3 otherwise.
 * https://oeis.org/A080574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n > 1: if n is odd and n already appears among
 *            {a(1),...,a(n-1)} → a(n) = a(n‑1) + 1 if n is even and n already
 *            appears among {a(1),...,a(n-1)} → a(n) = a(n‑1) + 2 otherwise →
 *            a(n) = a(n‑1) + 3
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Dynamic cache for the sequence ---------- */
static seq_t *seq_vals = NULL;      /* 1‑based storage: seq_vals[i] = a(i)  */
static size_t seq_cap = 0;          /* allocated capacity (number of slots)  */
static idx_t   seq_len = 0;         /* highest index computed so far        */

/* Ensure that seq_vals has room for at least 'needed' elements
   (indices 1..needed).  The returned array is 1‑based. */
static void ensure_capacity(size_t needed)
{
    if (needed < seq_cap) return;          /* already big enough */

    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap <= needed) new_cap <<= 1;

    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Binary search for 'target' in seq_vals[1..len].
   Returns true if found, false otherwise.  (len ≥ 0) */
static bool contains(const seq_t *arr, idx_t len, seq_t target)
{
    if (len == 0) return false;

    size_t low = 1, high = (size_t)len;  /* inclusive bounds */
    while (low <= high) {
        size_t mid = low + ((high - low) >> 1);
        seq_t v = arr[mid];
        if (v == target) return true;
        if (v < target)  low = mid + 1;
        else             high = mid - 1;
    }
    return false;
}

/* Public function required by the framework.  Computes a(n).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but keeps the function defined */

    /* Initialize the first term if not yet done. */
    if (seq_len == 0) {
        ensure_capacity(2);     /* room for indices 0 and 1 */
        seq_vals[1] = (seq_t)1;
        seq_len = 1;
    }

    if (n <= seq_len) return seq_vals[n];

    /* Generate missing terms up to n. */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t prev = seq_vals[i - 1];
        seq_t inc;

        if (contains(seq_vals, seq_len, (seq_t)i)) {
            inc = (i & 1) ? (seq_t)1 : (seq_t)2;
        } else {
            inc = (seq_t)3;
        }

        seq_t cur = prev + inc;
        ensure_capacity(i + 1);          /* indices are 1‑based */
        seq_vals[i] = cur;
        seq_len = i;
    }

    return seq_vals[n];
}
