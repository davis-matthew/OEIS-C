/*
 * A142354  A triangular sequence "representation" of the modulo 10 Integer
 *          field: t(+)(n,m)=Mod[n + m, 10]; t(x)(n,m)=Mod[n*m, 10];
 *          t(n,m)=Mod[t(=)(n,m)*t(X)(n,m),10].
 * https://oeis.org/A142354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = ((r + c) mod 10) * ((r * c) mod 10) mod 10 * where n ≥ 1,
 *            k = n – 1, (r, c) is determined by * r = floor((sqrt(8·k + 1) –
 *            1)/2) and c = k – r·(r+1)/2. * This implements the triangular
 *            representation of the * modulo‑10 integer field described by
 *            A142354
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Map linear index to triangular coordinates. */
    idx_t k = n - (idx_t)1;               /* 0‑based index */

    /* Approximate row r using floating point sqrt. */
    double dk = (double)k;
    unsigned long long r = (unsigned long long)((sqrt(8.0 * dk + 1.0) - 1.0) / 2.0);

    /* Correct possible off‑by‑one errors from floating point. */
    unsigned __int128 tri = ( (unsigned __int128)r * (r + 1ULL) ) / 2ULL;
    if (tri > (unsigned __int128)k) {
        while (tri > (unsigned __int128)k) {
            r--;
            tri = ( (unsigned __int128)r * (r + 1ULL) ) / 2ULL;
        }
    } else {
        while ( ( ( (unsigned __int128)(r + 1) * (r + 2ULL) ) / 2ULL ) <= (unsigned __int128)k ) {
            r++;
            tri = ( (unsigned __int128)r * (r + 1ULL) ) / 2ULL;
        }
    }

    idx_t col = k - (idx_t)tri;   /* 0 ≤ col ≤ r */

    /* Compute the value modulo 10. */
    unsigned long long r10 = r % 10ULL;
    unsigned long long c10 = col % 10ULL;
    unsigned long long sum10 = (r10 + c10) % 10ULL;
    unsigned long long prod10 = (r10 * c10) % 10ULL;
    unsigned long long res = (sum10 * prod10) % 10ULL;

    return (seq_t)res;
}
