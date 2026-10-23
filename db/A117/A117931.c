/*
 * A117931  Inductive sum-and-difference sequence.
 * https://oeis.org/A117931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2;
   for n>=3 odd: a(n)=least unused integer of the form x+y
                 with x,y distinct unused positive integers;
   for n>=3 even: a(n)=least unused difference x-y
                 with x,y already used positive integers. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Ensure that an unsigned char array has capacity at least 'need'.
   The array is resized to powers of two for efficiency. */
static void ensure_used_cap(size_t *cap, unsigned char **arr, size_t need)
{
    if (need < *cap)
        return;
    size_t new_cap = *cap ? *cap : 1;
    while (new_cap <= need)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(*arr, new_cap);
    if (!tmp) abort();
    for (size_t i = *cap; i < new_cap; ++i)
        tmp[i] = 0;
    *arr = tmp;
    *cap = new_cap;
}

/* Ensure that a seq_t array has capacity at least 'need'. */
static void ensure_seq_cap(size_t *cap, seq_t **seq, size_t need)
{
    if (need < *cap)
        return;
    size_t new_cap = *cap ? *cap : 1;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *seq = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* guard for out-of-range index */

    /* static state: generated terms and usage map */
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t *seq = NULL;

    static size_t used_cap = 0;
    static unsigned char *used = NULL;   /* 0=unused, 1=used */

    /* Grow global arrays to hold at least n terms */
    if (seq_len < (size_t)n) {
        ensure_seq_cap(&seq_cap, &seq, (size_t)n);

        if (used_cap == 0) {
            used_cap = 256;
            used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
            if (!used) abort();
        }
    }

    /* Compute terms until we reach n */
    for (; seq_len < (size_t)n; ++seq_len) {
        idx_t idx = (idx_t)seq_len + 1;   /* 1‑based position */
        seq_t val = 0;

        if (idx == 1) {
            val = 1;
        } else if (idx == 2) {
            val = 2;
        } else if (idx & 1) {   /* odd index: sum of two distinct unused numbers */
            for (idx_t s = 1; ; ++s) {
                ensure_used_cap(&used_cap, &used, (size_t)s);
                if (used[(size_t)s])                /* s already used */
                    continue;
                bool good = false;
                for (idx_t u = 1; u < s; ++u) {
                    idx_t v = s - u;
                    if (u == v)                     /* need distinct */
                        continue;
                    ensure_used_cap(&used_cap, &used, (size_t)u);
                    ensure_used_cap(&used_cap, &used, (size_t)v);
                    if (!used[(size_t)u] && !used[(size_t)v]) {
                        good = true;
                        break;
                    }
                }
                if (good) {
                    val = (seq_t)s;
                    break;
                }
            }
        } else {             /* even index: difference of two present numbers */
            for (idx_t d = 1; ; ++d) {
                ensure_used_cap(&used_cap, &used, (size_t)d);
                if (used[(size_t)d])                /* difference already used */
                    continue;
                bool good = false;
                for (size_t i = 0; i < seq_len; ++i) {
                    seq_t x = seq[i];
                    if (x <= (seq_t)d)               /* would give non‑positive diff */
                        continue;
                    seq_t y = x - (seq_t)d;          /* y > 0 */
                    ensure_used_cap(&used_cap, &used, (size_t)y);
                    if (used[(size_t)y]) {
                        good = true;
                        break;
                    }
                }
                if (good) {
                    val = (seq_t)d;
                    break;
                }
            }
        }

        /* Store the new term and mark it as used */
        seq[seq_len] = val;
        size_t v = (size_t)val;
        ensure_used_cap(&used_cap, &used, v);
        used[v] = 1;
    }

    return seq[seq_len - 1];
}
