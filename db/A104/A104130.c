/*
 * A104130  Numbers n such that n33 is prime and n is a multiple of ten.
 * https://oeis.org/A104130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth multiple of 10 such that a(n)33 is prime
 * Range:     u8 none, u16 n <= 13, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9113 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* enlarge array to store seq_t values */
static void add_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val) {
    if (*cap == 0) {
        *cap  = 4;
        *arr  = mem_new(seq_t, (*cap));
    }
    if (*size == *cap) {
        *cap <<= 1;
        *arr  = mem_resize(seq_t, *arr, (*cap));
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for out‑of‑range index */

    /* static cache for already found terms and the next candidate k */
    static seq_t *terms = NULL;
    static idx_t terms_cap = 0;
    static idx_t terms_size = 0;
    static idx_t next_k = 1;

    while (terms_size < n) {
        /* candidate multiple of ten   t = 10 * k  */
        seq_t t = ((seq_t)10 * next_k);

        /* number to test for primality: t33 = t * 100 + 33 */
        seq_t test = t * ((seq_t)100) + 33;

        if (is_prime_seq(test))
            add_seq(&terms, &terms_cap, &terms_size, t);

        ++next_k;
    }

    return terms[n - 1];  /* 1‑based index */
}
