/*
 * A080195  11-smooth numbers which are not 7-smooth.
 * https://oeis.org/A080195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11 * A051038(n)
 * Range:     u8 n <= 19, u16 n <= 423, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A051038 is the 11‑smooth numbers (primes 2,3,5,7,11).  */
#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1; guard against underflow */

    /* Allocate array for the first n 11‑smooth numbers */
    size_t sz = (size_t)n;
    seq_t *smooth = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!smooth)
        return 0; /* unlikely failure */

    smooth[0] = 1; /* first 11‑smooth number */

    size_t i2 = 0, i3 = 0, i5 = 0, i7 = 0, i11 = 0;

    for (size_t i = 1; i < sz; ++i) {
        seq_t v2  = smooth[i2] * (seq_t)2;
        seq_t v3  = smooth[i3] * (seq_t)3;
        seq_t v5  = smooth[i5] * (seq_t)5;
        seq_t v7  = smooth[i7] * (seq_t)7;
        seq_t v11 = smooth[i11] * (seq_t)11;

        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;
        if (v7 < next) next = v7;
        if (v11 < next) next = v11;

        smooth[i] = next;

        if (next == v2) ++i2;
        if (next == v3) ++i3;
        if (next == v5) ++i5;
        if (next == v7) ++i7;
        if (next == v11) ++i11;
    }

    seq_t result = smooth[sz - 1] * (seq_t)11;
    free(smooth);
    return result;
}
