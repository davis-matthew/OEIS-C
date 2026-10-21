/*
 * A055456  a(n) is the smallest number which is not the sum of exactly 1 or
 *          of n earlier terms.
 * https://oeis.org/A055456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-1 if n-1 > 0 and n-1 has not appeared among *
 *            a(1),…,a(n-1); otherwise a(n) = n^2 - n + 1. * The sequence is
 *            generated incrementally and stored so that * repeated calls to
 *            a(idx_t n) return the correct term for any * n ≥ 1 (calls may
 *            arrive in arbitrary order). The implementation * uses...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Forward declaration required by the build system. */
seq_t a(idx_t n);

/* -----------------  internal helpers  ----------------- */

static seq_t *seq_buf = NULL;      /* array holding computed terms      */
static size_t  seq_len = 0;        /* number of terms currently stored */
static size_t  seq_cap = 0;        /* allocated capacity of seq_buf    */

/* Ensure capacity for at least 'needed' entries. */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Linear search: returns true iff 'value' is among the first 'len'
 * elements of seq_buf.  This is O(len); for n ≤ 10 000 the total
 * work stays comfortably below the time limit. */
static bool contains(size_t len, seq_t value)
{
    for (size_t i = 0; i < len; ++i)
        if (seq_buf[i] == value) return true;
    return false;
}

/* Generate terms up to index 'n' (1‑based).  The function is
 * idempotent: repeated calls produce the same result. */
static void generate_up_to(idx_t n)
{
    while (seq_len < n) {
        idx_t cur_n = seq_len + 1;           /* next term index           */
        seq_t candidate = (seq_t)(cur_n - 1); /* n-1                      */
        seq_t next_term;

        /* Decide which value satisfies the definition. */
        if (candidate > 0 && !contains(seq_len, candidate)) {
            next_term = candidate;
        } else {
            /* Use 64‑bit arithmetic; overflow is safe for n ≤ 10 000.
             * The result is then cast to seq_t – in the 128‑bit case
             * this truncates to the lower 128 bits, which is exactly
             * the intended value. */
            unsigned long long prod = (unsigned long long)cur_n *
                                      (unsigned long long)cur_n;
            unsigned long long val  = prod - (unsigned long long)cur_n + 1ULL;
            next_term = (seq_t)val;
        }

        /* Store the new term. */
        ensure_capacity(seq_len + 1);
        seq_buf[seq_len] = next_term;
        seq_len++;
    }
}

/* ---------------  public entry point  ----------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n = 0, but safe stub */
    generate_up_to(n);
    return seq_buf[n - 1];
}
