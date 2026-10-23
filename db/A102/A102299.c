/*
 * A102299  Number of prime divisors with multiplicity of n where n and n+1
 *          are composite or twin composite numbers.
 * https://oeis.org/A102299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime, omega_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102299: a(n) = Ω(k) where k is the n‑th integer such that both
   k and k+1 are composite (i.e., consecutive composite numbers). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/omega_of.h"

/* Static storage for computed terms */
static seq_t *a_vals = NULL;   /* array of sequence values */
static idx_t a_len = 0;        /* number of values stored */
static idx_t a_cap = 0;        /* current allocation capacity */
static idx_t next_n = 2;       /* next integer to test */

/* Ensure the array can hold at least 'min_cap' elements. */
static void grow_array(idx_t min_cap) {
    if (min_cap <= a_cap) return;
    idx_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *new_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!new_vals) exit(1);    /* allocation failed */
    a_vals = new_vals;
    a_cap = new_cap;
}

/* Generate sequence terms up to 'target' index. */
static void fill_until(idx_t target) {
    if (a_len >= target) return;   /* already have enough terms */

    grow_array(target);            /* ensure capacity */
    for (; a_len < target; ++next_n) {
        if (next_n <= 1) continue; /* skip 0 and 1 */
        /* Check if both n and n+1 are composite (i.e., not prime). */
        if (!is_prime((uint64_t)next_n) &&
            (!is_prime((uint64_t)(next_n + 1)))) {
            a_vals[a_len++] = omega_of(next_n);
        }
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0; return 0 */
    fill_until(n);
    return a_vals[n - 1];
}
