/*
 * A076518  Numbers n such that Fibonacci(n) == sigma(n) (mod n).
 * https://oeis.org/A076518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3263, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_mod, fib_pair_mod, property_holds, sigma_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076518 – Numbers n such that Fibonacci(n) ≡ σ(n) (mod n)
   The function a(n) returns the n‑th such integer (1‑based indexing). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/fib_mod.h"
#include "../utils/fib_pair_mod.h"
#include "../utils/property_holds.h"
#include "../utils/sigma_mod.h"

/* dynamic storage of terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t tested_n = 0;

/* ensure capacity for terms vector */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t newcap = terms_cap == 0 ? 64 : terms_cap * 2;
    while (newcap < needed) newcap *= 2;
    terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    terms_cap = newcap;
}

/* main entry */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (terms_len < n) {
        ++tested_n;
        if (property_holds(tested_n)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)tested_n;
        }
    }
    return terms[n - 1];
}
