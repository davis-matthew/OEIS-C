/*
 * A152293  Primes of the form : (p-n)/(n+1)=prime and (n+1)*p+n=prime. n=3.
 * https://oeis.org/A152293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that (p-3)/4 and 4p+3 are prime (OEIS A152293, n = 3).
   The function returns the n‑th such prime (1‑based indexing). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t next_candidate = 2;

/* Append a value to the internal sequence array. */
static void seq_add(seq_t v) {
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 64;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = v;
}

/* Generate terms until we have at least n of them. */
static void ensure_sequence(idx_t n) {
    while (seq_len < n) {
        /* Find the next prime p >= next_candidate. */
        idx_t p = next_candidate;
        while (!is_prime_big((seq_t)p))
            ++p;

        /* Check the two derived primes. */
        if (p >= 3 && ((p - 3) & 3) == 0) {
            idx_t q = (p - 3) >> 2;        /* (p-3)/4 */
            if (is_prime_big((seq_t)q)) {
                seq_t r = (seq_t)4 * (seq_t)p + (seq_t)3; /* 4p+3 */
                if (is_prime_big(r))
                    seq_add((seq_t)p);
            }
        }

        /* Advance to the next possible prime. */
        next_candidate = p + 1;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety for out of bounds, offset is 1 */
    ensure_sequence(n);
    return seq[n - 1];
}
