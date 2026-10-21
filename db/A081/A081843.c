/*
 * A081843  a(1)=0, a(n)=a(n-1)+5 if n is already in the sequence,
 *          a(n)=a(n-1)+4 otherwise.
 * https://oeis.org/A081843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A081843
 * a(1) = 0
 * a(n) = a(n-1) + 5  if n is already a term of the sequence
 * a(n) = a(n-1) + 4  otherwise
 *
 * The implementation memoises the sequence and keeps a bit‑set of
 * values that have already appeared, so any successive calls may be
 * answered in O(1) after the initial fill up to the requested index.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ----- internal static state ----- */
static seq_t *seq_arr = NULL;          /* 1‑indexed values a(1), a(2), …   */
static size_t seq_cap = 0;             /* capacity of seq_arr               */
static size_t seq_len = 0;             /* largest index computed so far     */

static unsigned char *present_bits = NULL; /* bit i set ⇔ i has appeared as a term */
static size_t present_cap_bytes = 0;        /* number of bytes allocated for present_bits */

/* ----- bit helpers ----- */
#define GET_BIT(barr, idx)  (((barr)[(size_t)(idx)>>3] >> ((idx)&7)) & 1U)
#define SET_BIT(barr, idx)  ((barr)[(size_t)(idx)>>3] |= (1U << ((idx)&7)))

/* ----- sequence array helpers ----- */
static void ensure_seq_cap(idx_t need)
{
    size_t required = (size_t)need + 1;          /* indices 0 … need */
    if (required <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 2;
    while (new_cap < required) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!seq_arr) abort();
    seq_cap = new_cap;
}

/* ----- present bitset helpers ----- */
static void ensure_present_cap(idx_t need_val)
{
    size_t needed_bytes = (size_t)(need_val >> 3) + 1;
    if (needed_bytes <= present_cap_bytes) return;
    size_t new_bytes = present_cap_bytes ? present_cap_bytes * 2 : 2;
    while (new_bytes < needed_bytes) new_bytes <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(present_bits, new_bytes);
    if (!tmp) abort();
    if (new_bytes > present_cap_bytes)
        memset(tmp + present_cap_bytes, 0, new_bytes - present_cap_bytes);
    present_bits = tmp;
    present_cap_bytes = new_bytes;
}

/* ----- public function ----- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 – return 0 */

    /* initialise data structures on first call */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq_arr[1] = (seq_t)0;     /* a(1) = 0 */
        seq_len = 1;

        ensure_present_cap(0);
        SET_BIT(present_bits, (size_t)0); /* value 0 is present */
    }

    /* already computed up to n? */
    if (seq_len >= n) return seq_arr[n];

    /* make sure we have space for all indices up to n */
    ensure_seq_cap(n);

    /* generate terms up to n */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        bool in_prev = GET_BIT(present_bits, (size_t)i);
        seq_t inc = in_prev ? (seq_t)5 : (seq_t)4;
        seq_t val = seq_arr[i - 1] + inc;
        seq_arr[i] = val;

        /* mark this value as present for future queries */
        ensure_present_cap((idx_t)val);
        SET_BIT(present_bits, (size_t)val);
    }

    seq_len = n;
    return seq_arr[n];
}
