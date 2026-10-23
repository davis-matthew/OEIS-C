/*
 * A196538  Number of nX2 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors less than or equal to
 *          itself.
 * https://oeis.org/A196538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+22*a(n-2)+24*a(n-3)-86*a(n-4)-296*a(n-5)-126*a(n-6)+428*a(n-7)+420*a(n-8)+239*a(n-9)+92*a(n-10)-147*a(n-11)+22*a(n-12)+26*a(n-13)-13*a(n-14)+5*a(n-15)
 *            * Initial values: a(1)=3, a(2)=23, a(3)=56, a(4)=461, a(5)=2897,
 *            a(6)=17967, * a(7)=116911, a(8)=757146, a(9)=4893755,
 *            a(10)=31718...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *buf = NULL;
    static idx_t cap = 0;      /* current allocated capacity */
    static idx_t len = 0;      /* number of terms currently stored (len >= 15) */

    /* recurrence coefficients */
    static const int coeff[15] = {
        3, 22, 24, -86, -296, -126,
        428, 420, 239, 92, -147, 22,
        26, -13, 5
    };

    /* first 15 terms */
    static const seq_t init[15] = {
        3ull, 23ull, 56ull, 461ull, 2897ull,
        17967ull, 116911ull, 757146ull, 4893755ull,
        31718101ull, 205477601ull, 1331145835ull,
        8624488786ull, 55876739813ull, 362017883823ull
    };

    if (n == 0)
        return 0;                 /* undefined for n=0, return 0 for safety */

    /* initialise buffer on the first call */
    if (len == 0) {
        cap = 64; /* arbitrary initial capacity */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        for (int i = 0; i < 15; ++i)
            buf[i] = init[i];
        len = 15;
    }

    /* extend buffer if required */
    if (n > len) {
        if (n > cap) {
            /* grow capacity to at least n */
            while (cap < n)
                cap <<= 1;
            buf = (seq_t *)realloc(buf, cap * sizeof(seq_t));
        }
        /* compute terms from current len+1 up to n */
        for (idx_t cur = len + 1; cur <= n; ++cur) {
            seq_t pos = 0, neg = 0;
            for (int i = 0; i < 15; ++i) {
                seq_t val = buf[cur - 2 - i]; /* a(cur-i-1) */
                int c = coeff[i];
                if (c > 0)
                    pos += val * (seq_t)c;
                else if (c < 0)            /* negative coefficient */
                    neg += val * (seq_t)(-c);
            }
            buf[cur - 1] = pos - neg;
        }
        len = n;
    }

    return buf[n - 1];
}
