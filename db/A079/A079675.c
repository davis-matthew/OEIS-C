/*
 * A079675  a(1)=1; a(n)=sum(u=1,n-1,sum(v=1,u,sum(w=1,v,sum(x=1,
 *          w,sum(y=1,x,a(y)))))).
 * https://oeis.org/A079675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} binomial(5*n-4*k-1, k) (OEIS A079675)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n ≥ 7: a(n) = 5*a(n-1) – 4*a(n-2) + a(n-3) + b(n), */
/* where b(n) is the 6‑periodic sequence (0, 1, 1, 0, –1, –1). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values for a(1) … a(6). */
    static const seq_t base[6] = { (seq_t)1, (seq_t)1, (seq_t)6,
                                   (seq_t)26, (seq_t)106, (seq_t)431 };
    if (n <= 6)
        return base[n-1];

    /* Periodic correction term b(n): 0,1,1,0,-1,-1. */
    static const int8_t periodic[6] = { 0, 1, 1, 0, -1, -1 };

    /* Window holding a(n-3), a(n-2), a(n-1). */
    seq_t a0 = base[3]; /* a(4) */
    seq_t a1 = base[4]; /* a(5) */
    seq_t a2 = base[5]; /* a(6) */

    seq_t term = 0;          /* will hold a(n) */
    for (idx_t cur = 7; cur <= n; ++cur) {
        int8_t b = periodic[(cur - 1) % 6];

        term = 5 * a2;      /* 5*a(n-1) */
        term -= 4 * a1;     /* – 4*a(n-2) */
        term += a0;         /* + a(n-3) */
        if (b == 1)
            term += 1;      /* +1 if b(n) == 1 */
        else if (b == -1)
            term -= 1;      /* –1 if b(n) == –1 */

        /* Shift the window for next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = term;
    }
    return term;
}
