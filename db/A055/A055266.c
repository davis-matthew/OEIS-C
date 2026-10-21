/*
 * A055266  a(n) + a(n+1) is never prime; lexicographically earliest such
 *          sequence of distinct positive integers.
 * https://oeis.org/A055266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the lexicographically earliest sequence of distinct
 *            positive integers such that a(n)+a(n+1) is never prime. The
 *            sequence is constructed greedily: for each position n>1 pick the
 *            smallest unused integer k with a(n-1)+k not prime. This
 *            procedure produces the OEIS sequence A055266. The implem...
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities from the project */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *seq          = NULL;   /* sequence array */
static size_t seq_cap      = 0;      /* allocated elements */
static idx_t seq_len  = 0;           /* number of computed terms */

static bool *used          = NULL;   /* marks numbers already used */
static size_t used_cap     = 0;      /* allocated used array size */

/* Ensure the used array can index up to idx (inclusive).  Newly
   added entries are zeroed. */
static void ensure_used_cap(idx_t idx)
{
    if ((size_t)idx < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= (size_t)idx)
        new_cap *= 2;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort(); /* memory allocation failure treated as fatal */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Add a new element to the sequence array, expanding it if necessary. */
static void seq_append(seq_t val)
{
    if ((size_t)seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* Main sequence function.  Implements the greedy construction described
   above and memoises results so that any later call with smaller n is
   still fast. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* the offset is 1; value for n=0 is undefined */

    /* Grow the cache until it contains the requested term. */
    while (seq_len < n) {
        seq_t last = seq_len ? seq[seq_len - 1] : 0;
        idx_t candidate = 1;

        /* Find smallest unused integer not creating a prime sum with the
           previous term. */
        while (true) {
            if (candidate >= used_cap)
                ensure_used_cap(candidate);
            if (!used[candidate]) {
                if (last == 0 || !isPrimeGeneric((seq_t)last + (seq_t)candidate))
                    break;
            }
            ++candidate;
        }

        /* Record the chosen number. */
        used[candidate] = true;
        seq_append((seq_t)candidate);
    }

    return seq[n - 1];
}
