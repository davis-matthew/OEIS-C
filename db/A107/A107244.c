/*
 * A107244  Sum of squares of hexanacci numbers (A001592, Fibonacci 6-step
 *          numbers).
 * https://oeis.org/A107244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A107244: Sum of squares of hexanacci numbers (hexanacci n‑step Fibonacci)  */
/*  a(n) = F_6(0)^2 + F_6(1)^2 + … + F_6(n)^2,   a(0)=0,  a(n)=a(n-1)+F_6(n)^2  */

#include <stddef.h>   /* for NULL */
#include <stdlib.h>   /* for malloc, realloc */

static seq_t *F = NULL;      /* F[i] = hexanacci number F_6(i)      */
static seq_t *S = NULL;      /* S[i] = a(i) = sum_{k=0..i} F_6(k)^2 */
static idx_t capacity = 0;   /* allocated array length              */
static idx_t size = 0;       /* number of indices already computed   */

/* enlarge the arrays so that indices up to new_cap-1 are valid   */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= capacity)
        return;
    if (capacity == 0)
        capacity = 64;                 /* initial allocation */
    while (capacity < new_cap)
        capacity <<= 1;                /* grow multiplicatively */

    F = (seq_t *)realloc(F, capacity * sizeof(seq_t));
    S = (seq_t *)realloc(S, capacity * sizeof(seq_t));
}

/* compute F[i] and S[i] for all i from size up to and including n */
static void extend_to(idx_t n)
{
    idx_t i;
    for (i = size; i <= n; ++i) {
        seq_t f;
        if (i < 5)                    /* F_6(0)..F_6(4) = 0 */
            f = 0;
        else if (i == 5)               /* F_6(5) = 1 */
            f = 1;
        else {                         /* i >= 6: sum of previous 6 values */
            seq_t sum = 0;
            idx_t j;
            for (j = 1; j <= 6; ++j)
                sum += F[i - j];
            f = sum;
        }
        F[i] = f;
        if (i == 0)
            S[i] = f * f;              /* 0^2 */
        else
            S[i] = S[i - 1] + f * f;   /* accumulate square */
    }
    size = n + 1;                     /* now indices 0..n are computed */
}

/* public function required by the problem statement   */
seq_t a(idx_t n)
{
    if (size == 0) {                   /* first call: initialise arrays   */
        ensure_capacity(64);

        /* initialise F[0..4] = 0, F[5] = 1, S[0..5] accordingly */
        {
            idx_t j;
            for (j = 0; j < 5; ++j) {
                F[j] = 0;
                S[j] = 0;                /* 0^2 = 0 */
            }
        }
        F[5] = 1;
        S[5] = 1;                      /* 0^2+0^2+0^2+0^2+0^2+1^2 = 1 */

        size = 6;                      /* indices 0‑5 are ready          */
    }

    if (n >= size)                     /* lazily extend to requested n   */
        extend_to(n);

    return S[n];
}
