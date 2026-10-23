/*
 * A188645  Array of ((k^n)+(k^(-n)))/2 where k=(sqrt(x^2+1)+x)^2 for integers
 *          x>=1.
 * https://oeis.org/A188645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T_k(2*m^2+1) where m = 1 + floor((sqrt(8n+1)-1)/2) and k
 *            = n - m*(m-1)/2. This reads the Chebyshev array
 *            A(m,k)=T_k(2*m^2+1) by antidiagonals
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The types seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n) {
    /* Determine the diagonal number s (s>=1) such that
       s*(s+1)/2 > n.  For n up to 10^4, s <= 200, so a simple loop is
       inexpensive. */
    uint64_t nn = n;
    uint64_t s = 1;
    while ((s * (s + 1)) / 2 <= nn) {
        ++s;
    }

    /* Compute the row index r and column index c in the array:
       row r = 1 + offset within diagonal
       column c = (s-1) - offset */
    uint64_t prev = ((s - 1) * s) / 2;   /* cumulative terms before diagonal s */
    uint64_t d    = nn - prev;           /* zero‑based offset within diagonal */
    uint64_t r    = d + 1;               /* 1‑based row index */
    uint64_t c    = (s - 1) - d;         /* 0‑based column index */

    /* Compute the Chebyshev polynomial T_c(2*r^2 + 1) efficiently. */
    seq_t rr = (seq_t)r * (seq_t)r;          /* r^2 */
    seq_t x  = rr * 2 + 1;                   /* 2*r^2 + 1 */

    if (c == 0) {
        return 1;
    }
    if (c == 1) {
        return x;
    }

    seq_t t0 = 1;      /* T_0(x) */
    seq_t t1 = x;      /* T_1(x) */
    for (uint64_t i = 1; i < c; ++i) {
        /* t2 = 2 * x * t1 - t0  (Chebyshev recurrence) */
        seq_t t2 = (t1 + t1) * x - t0;
        t0 = t1;
        t1 = t2;
    }
    return t1;
}
