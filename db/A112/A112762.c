/*
 * A112762  Exponent of 5 (value of k) in n-th number of the form 2^i*3^j*5^k.
 * https://oeis.org/A112762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of 5 in the nth 5‑smooth number (2¹ᶦ·3ʲ·5ᵏ)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard against 0 */

    static unsigned __int128 *h = NULL;   /* sequence of 5‑smooth numbers */
    static size_t hcap = 0, hsize = 0;      /* capacity and current size */
    static seq_t *e = NULL;                /* corresponding exponent of 5 */
    static size_t ecap = 0, esize = 0;

    static idx_t i2 = 0, i3 = 0, i5 = 0;   /* merge pointers */

    /* initialise first element */
    if (hcap == 0) {
        hcap = 8;
        h = (unsigned __int128 *)malloc(hcap * sizeof(*h));
        ecap = 8;
        e = (seq_t *)malloc(ecap * sizeof(*e));
        h[0] = (unsigned __int128)1;
        e[0] = 0;
        hsize = esize = 1;
    }

    /* grow capacity if necessary */
    if (hsize < n) {
        size_t newcap = hcap < 8 ? 8 : hcap;
        while (newcap < (size_t)n) newcap <<= 1;

        unsigned __int128 *newh = (unsigned __int128 *)realloc(h, newcap * sizeof(*h));
        h = newh; hcap = newcap;
        seq_t *newe = (seq_t *)realloc(e, newcap * sizeof(*e));
        e = newe; ecap = newcap;
    }

    /* generate until we have at least n elements */
    while (hsize < n) {
        unsigned __int128 cand2 = h[i2] * (unsigned __int128)2ULL;
        unsigned __int128 cand3 = h[i3] * (unsigned __int128)3ULL;
        unsigned __int128 cand5 = h[i5] * (unsigned __int128)5ULL;

        unsigned __int128 next = cand2;
        if (cand3 < next) next = cand3;
        if (cand5 < next) next = cand5;

        h[hsize] = next;

        seq_t exp;
        if (next == cand5)
            exp = e[i5] + (seq_t)1;
        else if (next == cand2)
            exp = e[i2];
        else
            exp = e[i3];

        if (cand2 == next) ++i2;
        if (cand3 == next) ++i3;
        if (cand5 == next) ++i5;

        e[hsize] = exp;
        ++hsize;
    }

    return e[n-1];
}
