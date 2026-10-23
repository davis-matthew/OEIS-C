/*
 * A037377  Positive numbers k such that every base-2 digit of k is a base-8
 *          digit of k.
 * https://oeis.org/A037377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037377: numbers k such that every base‑2 digit of k (0 or 1)
   occurs as a base‑8 digit of k.  Thus the octal representation must
   contain 1, and if k is not of the form 2^m-1 (i.e. its binary form
   has a 0) it must also contain 0. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;   /* cached terms */
static idx_t  seq_count = 0;      /* number of cached terms   */
static idx_t  seq_cap   = 0;      /* capacity of seq_cache    */
static idx_t  last_k    = 0;      /* last k examined          */

/* test whether k satisfies the digit condition */
static bool valid_k(idx_t k)
{
    idx_t t = k;
    unsigned flags = 0;           /* bit i set if octal digit i appears */

    while (t != 0) {
        unsigned d = (unsigned)(t % 8);
        flags |= 1U << d;
        t /= 8;
        /* early exit if both 0 and 1 are seen */
        if ((flags & 0x3) == 0x3)
            /* still need to finish extracting higher digits only
               to avoid missing any later occurrences of 0 or 1, but
               they will not change the outcome */
            break;
    }

    /* If any higher octal digits remain, continue scanning to
       ensure we picked up all 0/1 digits. */
    while (t != 0) {
        unsigned d = (unsigned)(t % 8);
        flags |= 1U << d;
        t /= 8;
    }

    bool has_one  = (flags & 0x2) != 0;
    if (!has_one) return false;          /* missing digit 1 */

    bool has_zero = (flags & 0x1) != 0;

    /* binary_has_zero is true when k is NOT of the form 2^m-1 */
    bool only_ones = (((k + 1) & k) == 0);

    if (!only_ones && !has_zero) return false;  /* need zero in octal */
    return true;
}

/* append a new term to the cache, growing if necessary */
static void cache_append(seq_t value)
{
    if (seq_count == seq_cap) {
        idx_t new_cap = seq_cap == 0 ? 128 : seq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_count++] = value;
}

/* public function implementing the sequence */
seq_t a(idx_t n)
{
    while (seq_count < n) {
        ++last_k;
        if (valid_k(last_k))
            cache_append((seq_t)last_k);
    }
    return seq_cache[n - 1];
}
