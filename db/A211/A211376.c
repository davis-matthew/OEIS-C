/*
 * A211376  a(n) is the smallest 5-smooth number k such that both prime(n) - k
 *          and prime(n) + k are prime.
 * https://oeis.org/A211376
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 6716, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A211376
   a(n) = smallest 5‑smooth integer k such that prime(n)-k and prime(n)+k are prime.
   (Offset 3: a(3) is the first term.) */

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "../utils/get_prime.h"      /* get_prime(idx_t) – returns the k‑th prime */
#include "../utils/isPrimeGeneric.h" /* isPrimeGeneric(seq_t) – Miller–Rabin test */

static seq_t smallest_5smooth_k(seq_t p)
{
    /* Generate 5‑smooth numbers (Hamming numbers) in increasing order.
       Store them in a growing array.  The first element is 1.  */
    size_t cap = 1024;
    seq_t *h = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!h) return 0;              /* allocation failed – unlikely in tests */

    h[0] = 1;
    size_t cnt = 1;                /* number of entries in h */
    size_t i2 = 0, i3 = 0, i5 = 0;
    seq_t next2 = 2, next3 = 3, next5 = 5;

    while (1) {
        seq_t nxt = next2;
        if (next3 < nxt) nxt = next3;
        if (next5 < nxt) nxt = next5;

        /* Further candidates are >= p, thus impossible to satisfy prime-p‑k>0 */
        if (nxt >= p) {
            break;
        }

        /* Append nxt to the list, expanding array if needed. */
        if (cnt == cap) {
            cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(h, cap * sizeof(seq_t));
            if (!tmp) { free(h); return 0; }  /* allocation failed */
            h = tmp;
        }
        h[cnt++] = nxt;

        /* Test whether p-nxt and p+nxt are prime. */
        if (isPrimeGeneric(p - nxt) && isPrimeGeneric(p + nxt)) {
            free(h);
            return nxt;   /* found the smallest 5‑smooth k */
        }

        /* Advance pointers for the next Hamming number. */
        if (nxt == next2) { i2++; next2 = h[i2] * 2; }
        if (nxt == next3) { i3++; next3 = h[i3] * 3; }
        if (nxt == next5) { i5++; next5 = h[i5] * 5; }
    }

    free(h);
    return 0;   /* should not happen for n ≥ 3 because of the conjecture */
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* undefined for n < 3 */

    /* Obtain the n‑th prime.  For n ≤ 10000 it fits in 32 bits. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    return smallest_5smooth_k(p);
}
