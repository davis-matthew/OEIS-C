/*
 * A089088  a(0) = 1, a(1) = 2; for n > 1, a(n) = smallest positive number not
 *          already in sequence which has GCD > 1 with some earlier term.
 * https://oeis.org/A089088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the sequence by emitting composite numbers in
   natural order.  Whenever a composite is 2p with p prime, the prime
   p is inserted immediately after that composite.  The first two
   terms 1 and 2 are added explicitly.  This reproduces the OEIS
   sequence A089088. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state for memoisation */
static seq_t *seq   = NULL;   /* array of computed terms     */
static size_t seq_len  = 0;   /* number of terms currently stored */
static size_t seq_cap  = 0;   /* allocated size of seq         */

static bool *used   = NULL;  /* used[x] == true iff x already in the sequence */
static size_t used_len = 0;   /* allocated size of used          */

static idx_t current_composite = 4; /* next composite number to process */

/* Ensure used array can index at least [idx] */
static void ensure_used_capacity(size_t idx)
{
    if (idx < used_len) return;
    size_t new_len = used_len ? used_len * 2 : 1;
    while (new_len <= idx) new_len *= 2;
    bool *tmp = (bool *)realloc(used, new_len * sizeof(bool));
    if (!tmp) exit(EXIT_FAILURE);
    for (size_t i = used_len; i < new_len; ++i) tmp[i] = false;
    used = tmp;
    used_len = new_len;
}

/* Ensure seq array can store at least [new_len] elements */
static void ensure_seq_capacity(size_t new_len)
{
    if (seq_cap >= new_len) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < new_len) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* Simple primality test up to about 1e6 (fast for our use case). */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Initialise static structures on first call.  The first two terms
   1 and 2 are added explicitly. */
static void initialise(void)
{
    if (seq_len != 0) return;          /* already initialised */

    seq_cap = 32;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);

    ensure_used_capacity(2);
    used[1] = true;
    used[2] = true;

    seq[0] = (seq_t)1;
    seq[1] = (seq_t)2;
    seq_len = 2;

    /* ready to start emitting composites from 4 onward */
    current_composite = 4;
}

/* Append a single number to the sequence, extending arrays as needed. */
static void append_number(seq_t value)
{
    ensure_seq_capacity(seq_len + 1);
    seq[seq_len++] = value;

    enforce_used:
    ensure_used_capacity((size_t)value);
    used[(size_t)value] = true;
}

/* Generate terms until the sequence has at least n+1 elements. */
static void generate_upto(idx_t n)
{
    while ((size_t)n >= seq_len) {
        /* Emit the current composite */
        uint64_t comp = (uint64_t)current_composite;
        append_number((seq_t)comp);

        /* If the composite is 2p with p prime, insert p after it
           (skip p==2 because 2 is already in the sequence). */
        if ((comp & 1) == 0) {              /* comp even */
            uint64_t p = comp >> 1;
            if (p > 2 && !used[(size_t)p] && is_prime(p))
                append_number((seq_t)p);
        }

        /* Advance to the next composite number. */
        do {
            ++current_composite;
        } while (is_prime(current_composite));
    }
}

seq_t a(idx_t n)
{
    initialise();
    generate_upto(n);
    return seq[(size_t)n];
}
