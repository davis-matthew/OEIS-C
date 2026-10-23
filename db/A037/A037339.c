/*
 * A037339  Numbers whose base-2 and base-5 expansions have no digits in
 *          common.
 * https://oeis.org/A037339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate numbers whose base‑2 and base‑5 representations
   share no digits.  The function keeps a cache so that repeated calls
   with arbitrary order are fast. */
#include <stdbool.h>

static seq_t cache[20000];
static idx_t cache_cnt = 0;
static seq_t next_candidate = 2;

static bool valid_seq(seq_t v)
{
    if (v == 0) return false;           /* ignore zero */
    bool has_zero = false;
    seq_t t = v;
    while (t > 0) {
        seq_t d = t % (seq_t)5;
        if (d == (seq_t)1) return false;   /* digit 1 forbidden */
        if (d == (seq_t)0) has_zero = true;
        t /= (seq_t)5;
    }
    /* check for zeros in binary representation */
    bool bin_has_zero = ((v + (seq_t)1) & v) != (seq_t)0;
    if (bin_has_zero && has_zero) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range guard */
    while (cache_cnt < n) {
        while (!valid_seq(next_candidate)) next_candidate++;
        cache[cache_cnt++] = next_candidate;
        next_candidate++;
    }
    return cache[n - 1];
}
