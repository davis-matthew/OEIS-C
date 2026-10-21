/*
 * A086296  Euler's totient of 7-smooth numbers.
 * https://oeis.org/A086296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(A002473(n)) (Euler's totient of the n-th 7‑smooth
 *            number)
 * Range:     u8 n <= 85, u16 n <= 710, u32 n <= 7006, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised arrays for 7‑smooth numbers and their φ values          */
    static seq_t *smooth = NULL;
    static seq_t *phi    = NULL;
    static idx_t  cache_size = 0;          /* allocated array size       */
    static idx_t  computed   = 0;          /* how many entries already  */
    /* indices for the 4‑pointer smooth‑generator                      */
    static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    /* Allocate or grow the cache if necessary                        */
    if (smooth == NULL) {
        cache_size = n > 16 ? n : 16;
        smooth = (seq_t *)malloc(cache_size * sizeof(seq_t));
        phi    = (seq_t *)malloc(cache_size * sizeof(seq_t));
    } else if (n > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n) new_size = n;
        smooth = (seq_t *)realloc(smooth, new_size * sizeof(seq_t));
        phi    = (seq_t *)realloc(phi,    new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* If the requested term was already generated, return it         */
    if (n <= computed) return phi[n - 1];

    /* Initialise the generator on the first request                 */
    if (computed == 0) {
        smooth[0] = (seq_t)1;
        phi[0]    = (seq_t)1;              /* φ(1) = 1                 */
        computed = 1;
    }

    /* Generate smooth numbers and their φ values up to n             */
    while (computed < n) {
        seq_t c2 = smooth[i2] * (seq_t)2;
        seq_t c3 = smooth[i3] * (seq_t)3;
        seq_t c5 = smooth[i5] * (seq_t)5;
        seq_t c7 = smooth[i7] * (seq_t)7;

        seq_t next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        /* advance all pointers whose candidate equals the chosen one */
        if (next == c2) i2++;
        if (next == c3) i3++;
        if (next == c5) i5++;
        if (next == c7) i7++;

        smooth[computed] = next;

        /* compute φ(next) using the distinct primes 2,3,5,7          */
        seq_t phi_val = next;
        if (next % (seq_t)2 == (seq_t)0) phi_val = phi_val / (seq_t)2; /* *1 */
        if (next % (seq_t)3 == (seq_t)0) phi_val = phi_val / (seq_t)3 * (seq_t)2;
        if (next % (seq_t)5 == (seq_t)0) phi_val = phi_val / (seq_t)5 * (seq_t)4;
        if (next % (seq_t)7 == (seq_t)0) phi_val = phi_val / (seq_t)7 * (seq_t)6;

        phi[computed] = phi_val;
        computed++;
    }

    return phi[n - 1];
}
