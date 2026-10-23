/*
 * A101544  Smallest permutation of the natural numbers with a(3*k-2) +
 *          a(3*k-1) = a(3*k), k > 0.
 * https://oeis.org/A101544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A101544 – the smallest permutation of ℕ with
   a(3k‑2)+a(3k‑1)=a(3k).  The sequence is built block‑wise:
   repeatedly pick the two smallest unused integers a, b,
   insert them in the next two positions and append a+b.   */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *seq = NULL;          /* a(1), a(2), … */
static size_t seq_len = 0;         /* number of values stored   */
static size_t seq_cap = 0;         /* capacity of seq array     */

static uint8_t *used = NULL;       /* bitset of used values     */
static size_t used_len = 0;        /* array length in bytes     */

static seq_t next_candidate = 1;   /* next integer to test       */

/* enlarge the used array so that index v is valid; ignore overflow */
static void ensure_used_size(seq_t v)
{
    size_t idx = (size_t)v;
    if (v > (seq_t)SIZE_MAX) return;              /* safety guard     */
    if (idx >= used_len) {
        size_t newlen = used_len ? used_len * 2 : 128;
        while (newlen <= idx) newlen <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(used, newlen);
        if (!tmp) abort();                       /* memory error    */
        memset(tmp + used_len, 0, newlen - used_len);
        used = tmp;
        used_len = newlen;
    }
}

/* extend the sequence until it contains at least n terms (1‑based) */
static void extend_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* first unused integer a */
        while (1) {
            ensure_used_size(next_candidate);
            if (!used[(size_t)next_candidate]) break;
            ++next_candidate;
        }
        seq_t a = next_candidate;
        used[(size_t)a] = 1;

        /* second unused integer b > a */
        ++next_candidate;
        while (1) {
            ensure_used_size(next_candidate);
            if (!used[(size_t)next_candidate]) break;
            ++next_candidate;
        }
        seq_t b = next_candidate;
        used[(size_t)b] = 1;

        /* third integer c = a + b */
        seq_t c = a + b;
        ensure_used_size(c);
        used[(size_t)c] = 1;     /* mark as used (was unused) */

        /* store the block a, b, c */
        if (seq_len + 3 > seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!tmp) abort();                     /* memory error  */
            seq = tmp;
        }
        seq[seq_len++] = a;
        seq[seq_len++] = b;
        seq[seq_len++] = c;
    }
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* not used for offset 1   */
    if (!seq) {                                /* first call – init static data */
        seq = NULL;
        seq_len = 0;
        seq_cap = 0;
        used = NULL;
        used_len = 0;
    }
    extend_to(n);
    return seq[(size_t)(n - 1)];
}
