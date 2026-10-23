/*
 * A037411  Positive numbers having the same set of digits in base 2 and base
 *          6.
 * https://oeis.org/A037411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑6 digits are only 0 or 1
 *            and whose set of digits in base‑2 equals that set. The term a(n)
 *            is obtained by scanning base‑6 “binary” numbers (super‑numbers
 *            with digits 0/1) in increasing order and keeping those for which
 *            the binary representation contains exac...
 * Range:     u8 n <= 12, u16 n <= 122, u32 n <= 8180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* cache the sequence values that have already been computed */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* state of the generator: last base‑6 “binary” number processed */
    static idx_t last_b = 0;

    if (n == 0) return (seq_t)0;          /* sequence starts at a(1) */

    /* grow cache if necessary to hold the result for index n */
    while (cache_size < (size_t)n) {
        /* move to the next base‑6 “binary” number */
        ++last_b;
        idx_t tmp_b = last_b;

        /* compute its decimal value N = Σ bit_k * 6^k */
        seq_t N = 0;
        seq_t pow6 = 1;          /* 6^0 initially */
        while (tmp_b) {
            if (tmp_b & 1) N += pow6;
            pow6 *= (seq_t)6;
            tmp_b >>= 1;
        }

        /* check that all base‑6 digits are ≤ 1 and record if a zero appears */
        bool zero_present = false;
        seq_t t = N;
        bool all_digits_ok = true;
        while (t) {
            uint8_t d = (uint8_t)(t % (seq_t)6);
            if (d > 1) { all_digits_ok = false; break; }
            if (d == 0) zero_present = true;
            t /= (seq_t)6;
        }

        if (!all_digits_ok) continue;   /* skip this N */

        /* determine if the binary representation is all-ones (i.e. 2^k‑1) */
        bool all_ones_bin = ((N & (N + (seq_t)1)) == (seq_t)0);

        /* the sets of digits are equal iff they obey the following */
        bool ok = (zero_present ? (!all_ones_bin) : all_ones_bin);
        if (!ok) continue;  /* not a term of the sequence */

        /* store the term */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = cache_cap ?: 16;
            new_cap <<= 1;                     /* double the capacity */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                 /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = N;
    }

    return cache[n - 1];
}
