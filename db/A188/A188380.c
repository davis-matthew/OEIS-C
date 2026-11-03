/*
 * A188380  Balanced ternary Keith numbers.
 * https://oeis.org/A188380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is the n‑th positive integer N whose balanced ternary
 *            digits d1,…,dk (with d1 least significant) satisfy the k‑adic
 *            recurrence s_i = d_i (1 ≤ i ≤ k) s_i = s_{i‑k}+…+s_{i‑1} (i > k)
 *            the sequence eventually generates N
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* Helper: test whether N is a balanced‑ternary Keith number.  */
static bool is_balanced_ternary_keith(seq_t N)
{
    /* 1. Compute balanced ternary digits, least significant first.  */
    int8_t digits[64];                /* up to ~64 digits for 64‑bit values.  */
    size_t dlen = 0;
    seq_t x = N;

    if (x == 0) {
        digits[dlen++] = 0;
    } else {
        while (x > (seq_t)0 && dlen < sizeof(digits)) {
            seq_t rem = x % (seq_t)3;
            seq_t q   = x / (seq_t)3;
            if (rem == (seq_t)2) {
                rem = (seq_t)-1;
                q += (seq_t)1;
            }
            digits[dlen++] = (int8_t)rem;
            x = q;
        }
    }
    if (dlen == 0)
        return false;    /* should not happen */

    /* 2. Quick reject if digit sum ≤ 0.  */
    int sum_digits = 0;
    for (size_t i = 0; i < dlen; ++i)
        sum_digits += digits[i];
    if (sum_digits <= 0)
        return false;

    /* 3. Copy digits to a buffer in most‑significant‑first order.  */
    int64_t buf[64];
    int64_t sum = 0;
    int neg_count = 0;
    for (size_t i = 0; i < dlen; ++i) {
        int64_t val = digits[dlen - 1 - i];   /* reverse order */
        buf[i] = val;
        sum += val;
        if (val < 0)
            ++neg_count;
    }

    /* 4. Run the recurrence.  */
    size_t pos = 0;  /* index of the oldest term in the circular buffer.  */
    while (true) {
        int64_t next = sum;   /* next term is the sum of last dlen terms.  */

        if (next >= 0) {
            seq_t next_seq = (seq_t)next;
            if (next_seq == N)
                return true;
            if (next_seq > N && neg_count == 0)
                return false;
        }

        /* Slide the window: replace oldest term.  */
        int64_t old = buf[pos];
        buf[pos] = next;
        sum = sum - old + next;
        if (old < 0)
            --neg_count;
        if (next < 0)
            ++neg_count;

        pos = (pos + 1) % dlen;
    }
}

/* Forward declaration.  */
static bool is_balanced_ternary_keith(seq_t);

/* Main sequence accessor.  */
seq_t a(idx_t n)
{
    /* Static state.  */
    static seq_t *seq = NULL;     /* array of found Keith numbers      */
    static size_t len = 0;        /* number of computed terms         */
    static size_t cap = 0;        /* allocated capacity              */
    static idx_t last_checked = 0;/* last candidate examined          */

    if (n == 0)
        return 0; /* undefined for offset 1 */

    /* Generate more terms if needed.  */
    if (len < (size_t)n) {
        size_t target = (size_t)n;
        while (len < target) {
            seq_t cand = (seq_t)(last_checked + 1);
            ++last_checked;

            /* Skip the trivial case N == 1.  */
            if (cand == (seq_t)1)
                continue;

            if (is_balanced_ternary_keith(cand)) {
                /* Grow buffer if necessary.  */
                if (len == cap) {
                    size_t new_cap = (cap == 0) ? 16 : cap * 2;
                    seq_t *tmp = seq_alloc_array(new_cap);
                    for (size_t i = 0; i < len; ++i)
                        tmp[i] = seq[i];
                    if (seq)
                        seq_free_array(seq);
                    seq = tmp;
                    cap = new_cap;
                }
                seq[len++] = cand;
            }
        }
    }
    return seq[n - 1];
}
