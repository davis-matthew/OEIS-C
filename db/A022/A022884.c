/*
 * A022884  Numbers k such that prime(k) + prime(k+3) = prime(k+1) +
 *          prime(k+2).
 * https://oeis.org/A022884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 5824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A022884: indices k such that prime(k)+prime(k+3)=prime(k+1)+prime(k+2) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static idx_t *found         = NULL;   /* stored qualifying k */
static size_t found_count   = 0;       /* number stored */
static size_t found_cap     = 0;       /* allocation capacity */
static idx_t last_k         = 0;       /* last k tested */

/* Append a new qualifying k to the stored array, expanding if needed. */
static void add_found(idx_t k)
{
    if (found_count == found_cap) {
        size_t new_cap = found_cap == 0 ? 4 : found_cap * 2;
        idx_t *tmp = (idx_t *)realloc(found, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        found = tmp;
        found_cap = new_cap;
    }
    found[found_count++] = k;
}

/* External function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, guard */
    while (found_count < (size_t)n) {
        idx_t k = ++last_k;   /* next k to test */
        seq_t pk  = (seq_t)get_prime(k);
        seq_t pk1 = (seq_t)get_prime(k + 1);
        seq_t pk2 = (seq_t)get_prime(k + 2);
        seq_t pk3 = (seq_t)get_prime(k + 3);
        if (pk + pk3 == pk1 + pk2) {
            add_found(k);
        }
    }
    return (seq_t)found[n - 1];
}
