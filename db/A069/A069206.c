/*
 * A069206  Let m(n,k) be the sequence defined by m(n,1)=1, m(n,2)=n,
 *          m(n,k+2)=(m(n,k+1)+m(n,k))/2 if m(n,k+1)+m(n,k) is even,
 *          m(n,k+2)=m(n,k+1)-m(n,k) otherwise. Sequence gives the
 *          (experimentally unique) solution to m(n,x)=0 (for k > this
 *          solution m(n,k) is constant = +1 or -1 depending on n) or a(n)=0
 *          if...
 * https://oeis.org/A069206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that m(n,k)=0, where m(n,1)=1,
 *            m(n,2)=n, m(n,k+2) = (m(n,k+1)+m(n,k))/2 if the sum is even
 *            m(n,k+1)-m(n,k) otherwise. If no such k exists, a(n)=0
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t, idx_t is defined by the build */

/* One light‑weight cycle detector – the sequence never needs more
   than a few thousand steps, so a fixed array is sufficient. */
static const size_t MAX_STEPS = 20000;

seq_t a(idx_t n)
{
    /* Use an enlarged signed type for intermediate arithmetic.
       __int128 is available in both C11 and C++17 implementations
       used by the test harness. */
    __int128 prev = 1;
    __int128 curr = ( __int128 )n;
    idx_t k = 2;          /* current index of 'curr' */

    /* Store observed (prev, curr) pairs to detect repetition. */
    struct pair { __int128 a, b; } seen[MAX_STEPS];
    size_t seen_cnt = 0;
    seen[seen_cnt++] = (struct pair){prev, curr};

    while (1) {
        __int128 sum  = prev + curr;
        __int128 next;

        if ((sum & 1) == 0)          /* sum even */
            next = sum / 2;
        else                         /* sum odd */
            next = curr - prev;

        ++k;

        if (next == 0)                /* zero reached */
            return (seq_t)k;

        /* Detect a repeated pair → cycle → no solution. */
        for (size_t i = 0; i < seen_cnt; ++i) {
            if (seen[i].a == curr && seen[i].b == next)
                return (seq_t)0;
        }

        if (seen_cnt == MAX_STEPS)    /* safety guard */
            return (seq_t)0;

        /* Record new state and advance. */
        seen[seen_cnt++] = (struct pair){curr, next};
        prev = curr;
        curr = next;
    }
}
