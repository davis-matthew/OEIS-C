/*
 * A080887  Boolean XOR of (0,1) versions of Thue-Morse word (A010060) and
 *          Fibonacci word (A003849).
 * https://oeis.org/A080887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = popcount(n-1) mod 2 XOR FibWordBit(n-1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   parity_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Boolean XOR of the Thue‑Morse and Fibonacci words.
   a(1) = 0 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/parity_popcount.h"

/* Static cache of Fibonacci word lengths (Wk = length of the Fibonacci word
   at rank k).  The longest needed length grows with n; this cache allows
   successive calls to reuse previously computed values. */
static idx_t *fib_len = NULL;      /* array of lengths */
static size_t fib_len_size = 0;    /* number of stored lengths */
static size_t fib_len_cap  = 0;    /* allocated capacity */

/* Ensure that fib_len[] contains values up to at least n (inclusive). */
static void ensure_fib_len(idx_t n)
{
    if (fib_len == NULL) {
        fib_len_cap = 16;
        fib_len = (idx_t *)malloc(fib_len_cap * sizeof(idx_t));
        if (!fib_len) return;                 /* allocation failure */
        fib_len[0] = 1;                       /* W0 = "0" */
        fib_len[1] = 2;                       /* W1 = "01" */
        fib_len_size = 2;
    }

    /* Extend until the last stored length exceeds n. */
    while (fib_len_size == 0 || fib_len[fib_len_size - 1] <= n) {
        idx_t a = fib_len[fib_len_size - 1];
        idx_t b = fib_len[fib_len_size - 2];
        if (a > UINT64_MAX - b) break;        /* avoid overflow */
        idx_t next = a + b;
        if (fib_len_size == fib_len_cap) {
            size_t new_cap = fib_len_cap * 2;
            idx_t *tmp = (idx_t *)realloc(fib_len, new_cap * sizeof(idx_t));
            if (!tmp) break;
            fib_len = tmp;
            fib_len_cap = new_cap;
        }
        fib_len[fib_len_size++] = next;
    }
}

/* Return the 0‑indexed bit of the Fibonacci word at position pos
   (0 or 1). Uses iterative descent from precomputed lengths. */
static int fib_word_bit(idx_t pos)
{
    /* Find smallest k with pos < fib_len[k]. */
    size_t k = 0;
    while (k < fib_len_size && pos >= fib_len[k]) k++;

    /* Descend using the recurrence Wk = W(k-1) + W(k-2). */
    while (k > 1) {
        if (pos < fib_len[k - 1]) {
            k -= 1;                 /* left part */
        } else {
            pos -= fib_len[k - 1];  /* right part */
            k -= 2;
        }
    }

    if (k == 0)
        return 0;                    /* W0 = "0" */
    /* k == 1: W1 = "01" */
    return (pos == 0) ? 0 : 1;
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* out of range safety guard */

    idx_t pos = n - 1;              /* 0‑based position */

    int thue = parity_popcount(pos);    /* parity of popcount */

    ensure_fib_len(pos);
    int fibb = fib_word_bit(pos);

    return (seq_t)(thue ^ fibb);
}
