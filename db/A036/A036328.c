/*
 * A036328  Composite numbers n such that juxtaposition of prime factors of n
 *          has length 4.
 * https://oeis.org/A036328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 1048, u32 n <= 1048, u64 n <= 1048,
 *            u128 n <= 1048, big any n
 * Helpers:   _smallest_prime_factor, digits_of, ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: nth composite integer whose prime factors, concatenated in decimal,
   produce a 4‑digit string. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"           /* bool is_prime(idx_t x); */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t n); */
#include "../utils/digits_of.h"            /* int digits_of(idx_t x); */
#include "../utils/ensure_capacity.h"      /* void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size); */

static seq_t *seq = NULL;        /* dynamically grown array of sequence terms */
static size_t seq_len   = 0;     /* number of terms stored so far */
static size_t seq_cap   = 0;     /* capacity of seq array */
static idx_t  next_num  = 1;     /* next integer to test */

/* Return nth term (1‑based) of sequence A036328. */
seq_t a(idx_t n)
{
    while (seq_len < (size_t)n) {
        next_num++;                    /* move to next integer */
        if (is_prime(next_num))        /* skip primes */
            continue;

        /* compute the total length of the concatenated prime factors */
        uint64_t tmp = (uint64_t)next_num;
        int len = 0;
        while (tmp > 1) {
            uint64_t spf = _smallest_prime_factor(tmp);
            tmp /= spf;
            len += digits_of(spf);
        }

        if (len == 4) {                /* desired property */
            ensure_capacity(seq_len + 1, &seq_cap, (void**)&seq, sizeof(seq_t));
            seq[seq_len++] = (seq_t)next_num;
        }
    }
    return seq[(size_t)n - 1];
}
