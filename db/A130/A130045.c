/*
 * A130045  Denominator of polynomial a[1]=1, a[2]->1+1/(x*a[1]),
 *          a[3]->1+1/(2*x*a[2]), a[4]->1+1/(3*x*a[3]),.. giving
 *          1,(1+x)/x,(3+2*x)/(2*(1+x)),(2+11*x+6*x^2)/(3*x*(3+2*x)), .. at
 *          x-> -1. Absolute values are equal to A067078(n)/n.
 * https://oeis.org/A130045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(1)=1, a(2)=1; for n≥3 a(n) = (n‑1) * ( Σ_{k=0}^{n‑3} k! + 1 )
 *            Sign pattern: for n≥3 the sign is negative when floor((n‑2)/2)
 *            is odd, otherwise positive (n=1,2 are always positive)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* PRE: seq_t and idx_t are provided by the build system. */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 1 || n == 2)
        return 1;                     /* first two terms */

    /* Static storage for cumulative factorial sums: S[i] = Σ_{k=0}^{i} k! */
    static seq_t *S = NULL;
    static size_t S_cap = 0;
    static idx_t  S_len = 0;          /* number of elements in S (S_len>0 when init) */
    static seq_t last_fact = 1;       /* factorial of (S_len-1) */

    idx_t need = n - 3;               /* index of the required sum element */

    /* Initialise on first call */
    if (S_len == 0) {
        S_cap = 4;                    /* initial capacity */
        S = (seq_t*)malloc(S_cap * sizeof(seq_t));
        S[0] = (seq_t)1;              /* 0! */
        S_len = 1;
        last_fact = (seq_t)1;         /* factorial of 0! */
    }

    /* Expand arrays if necessary */
    if (S_len <= need) {
        /* ensure capacity */
        while (S_len <= need) {
            if (S_len >= S_cap) {     /* grow */
                size_t new_cap = S_cap ? S_cap * 2 : 4;
                while (new_cap <= (size_t)need) new_cap <<= 1;
                S_cap = new_cap;
                S = (seq_t*)realloc(S, S_cap * sizeof(seq_t));
            }
            idx_t i = S_len;           /* next factorial index (i≥1) */
            last_fact = last_fact * (seq_t)i;
            S[S_len] = S[S_len - 1] + last_fact; /* cumulative sum up to i */
            S_len++;
        }
    }

    /* Core computation: a(n) = (n-1)*(S[need] + 1) with appropriate sign */
    seq_t val = (seq_t)(n - 1) * (S[need] + (seq_t)1);

    /* Apply sign: negative when floor((n-2)/2) is odd */
    if (((n - 2) / 2) & 1)
        val = -val;

    return val;
}
