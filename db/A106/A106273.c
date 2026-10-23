/*
 * A106273  Discriminant of the polynomial x^n - x^(n-1) - ... - x - 1.
 * https://oeis.org/A106273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n*(n+1)/2) * ((n+1)^(n+1) - 2*(2*n)^n) / (n-1)^2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special case: a(1) = 1 */

seq_t a(idx_t n)
{
    if (n == 1)                /* avoid division by zero */
        return 1;

    /* For small seq_t (≤64 bits) use 128‑bit intermediates to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(long long)) {
        /* use unsigned __int128 for large positive powers,
           then cast back to signed __int128 for subtraction. */
        unsigned __int128 nn = n;
        unsigned __int128 n1   = nn + 1;          /* (n+1)          */
        unsigned __int128 pow1 = 1;               /* (n+1)^(n+1)    */
        unsigned __int128 base1 = n1;
        idx_t e1 = n + 1;
        while (e1 > 0) {
            if (e1 & 1)
                pow1 *= base1;
            e1 >>= 1;
            if (e1)
                base1 *= base1;
        }

        unsigned __int128 pow2 = 1;               /* (2*n)^n         */
        unsigned __int128 base2 = nn * 2;
        idx_t e2 = n;
        while (e2 > 0) {
            if (e2 & 1)
                pow2 *= base2;
            e2 >>= 1;
            if (e2)
                base2 *= base2;
        }

        /* signed result of the numerator can be negative */
        __int128 num = (__int128)pow1 - ((__int128)pow2 * 2);
        unsigned __int128 den = (nn - 1) * (nn - 1);

        __int128 res128 = num / (__int128)den;         /* exact division */

        /* sign factor (-1)^(n*(n+1)/2) */
        switch (n % 4) {
            case 1:
            case 2:
                res128 = -res128;
                break;
            default:
                break;
        }

        return (seq_t)res128;
    }

    /* Otherwise seq_t is an arbitrary‑precision integer type.
       Compute directly with seq_t operations. */
    seq_t nn   = n;
    seq_t n1   = nn + 1;

    seq_t pow1 = 1;               /* (n+1)^(n+1)    */
    seq_t base1 = n1;
    idx_t e1 = n + 1;
    while (e1 > 0) {
        if (e1 & 1)
            pow1 *= base1;
        e1 >>= 1;
        if (e1)
            base1 *= base1;
    }

    seq_t pow2 = 1;               /* (2*n)^n        */
    seq_t base2 = (seq_t)2 * nn;
    idx_t e2 = n;
    while (e2 > 0) {
        if (e2 & 1)
            pow2 *= base2;
        e2 >>= 1;
        if (e2)
            base2 *= base2;
    }

    seq_t numerator   = pow1 - (pow2 * 2);
    seq_t denominator = (nn - 1) * (nn - 1);
    seq_t res = numerator / denominator;                 /* exact */

    /* sign factor (-1)^(n*(n+1)/2) */
    switch (n % 4) {
        case 1:
        case 2:
            res = -res;
            break;
        default:
            break;
    }
    return res;
}
