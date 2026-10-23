/*
 * A172298  a(n) = A027641(n) * A164555(n).
 * https://oeis.org/A172298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (numerator of Bernoulli number B_n)^2 (with a(1) = -1;
 *            odd n>1 give 0)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 27, i128 n <= 34,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Internal rational representation. */
typedef struct {
    seq_t num;
    seq_t den;   /* always positive after reduction */
} Rat;

/* Absolute value for seq_t. */
static inline seq_t abs_seq_t(seq_t x) { return x < 0 ? -x : x; }

/* Reduce a rational to lowest terms; the denominator stays positive. */
static void rat_reduce(Rat *r)
{
    if (r->num == (seq_t)0) {
        r->den = (seq_t)1;
        return;
    }
    seq_t g = gcd(abs_seq_t(r->num), r->den);
    r->num = r->num / g;
    r->den = r->den / g;
    if (r->den < (seq_t)0) {   /* keep denominator positive */
        r->num = -r->num;
        r->den = -r->den;
    }
}

/* Subtract two rationals: res = a - b.  Result is reduced. */
static void rat_subtract(Rat *res, const Rat *a, const Rat *b)
{
    res->num = a->num * b->den - b->num * a->den;
    res->den = a->den * b->den;
    rat_reduce(res);
}

/* Compute a(n) as defined. */
seq_t a(idx_t n)
{
    /* Static cache of computed Bernoulli numbers. */
    static idx_t computed = 0;          /* highest index computed */
    static size_t capacity = 0;         /* allocated size of arrays    */
    static Rat  *A = NULL;              /* temporary array for algorithm */
    static Rat  *B = NULL;              /* array of Bernoulli numbers   */

    /* Allocate minimal required space. */
    if (capacity == 0) {
        capacity = 1;
        A = (Rat *)malloc(capacity * sizeof(Rat));
        B = (Rat *)malloc(capacity * sizeof(Rat));
    }
    if ((idx_t)(capacity) <= n) {
        size_t newcap = (size_t)(n + 1);
        A = (Rat *)realloc(A, newcap * sizeof(Rat));
        B = (Rat *)realloc(B, newcap * sizeof(Rat));
        capacity = newcap;
    }

    /* Initialise B[0] if this is the first use. */
    if (computed == 0) {
        A[0].num = (seq_t)1;
        A[0].den = (seq_t)1;
        rat_reduce(&A[0]);
        B[0] = A[0];
        computed = 0;
    }

    /* Compute Bernoulli numbers up to n if needed. */
    for (idx_t m = computed + 1; m <= n; ++m) {
        /* A[m] = 1/(m+1). */
        A[m].num = (seq_t)1;
        A[m].den = (seq_t)(m + 1);
        rat_reduce(&A[m]);

        /* Akiyama–Tanigawa inner loop. */
        for (idx_t j = m; j > 0; --j) {
            Rat diff;
            rat_subtract(&diff, &A[j - 1], &A[j]);      /* A[j-1] - A[j] */
            diff.num *= (seq_t)j;                       /* multiply by j */
            rat_reduce(&diff);
            A[j - 1] = diff;
        }

        /* Store the Bernoulli number B_m. */
        B[m].num = A[0].num;
        B[m].den = A[0].den;
    }
    computed = n;

    /* Special case n = 1 */
    if (n == (idx_t)1)
        return (seq_t)-1;

    /* For odd n > 1 the numerator is already 0, so square yields 0. */
    seq_t num = B[n].num;
    return num * num;
}
